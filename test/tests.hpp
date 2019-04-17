#pragma once
#include <catch.hpp>
#include <csv/reader.hpp>

TEST_CASE("Parse an empty CSV", "[simple csv]") {
  csv::Reader csv;
  csv.read("inputs/empty.csv");
  auto rows = csv.rows();
  auto cols = csv.cols();
  REQUIRE(rows.size() == 0);
  REQUIRE(cols.size() == 0);
}

TEST_CASE("Parse file that doesn't exist", "[simple csv]") {
  csv::Reader csv;
  bool exception_thrown = false;
  try {
    csv.read("inputs/missing.csv");
  }
  catch (std::exception&) {
    exception_thrown = true;
  }
  REQUIRE(exception_thrown);
}

TEST_CASE("Parse the most basic of CSV buffers", "[simple csv]") {
  csv::Reader csv;
  csv.read("inputs/test_01.csv");
  auto rows = csv.rows();
  REQUIRE(rows.size() == 2);
  REQUIRE(rows[0]["a"] == "1");
  REQUIRE(rows[0]["b"] == "2");
  REQUIRE(rows[0]["c"] == "3");
  REQUIRE(rows[1]["a"] == "4");
  REQUIRE(rows[1]["b"] == "5");
  REQUIRE(rows[1]["c"] == "6");
}

TEST_CASE("Parse the most basic of CSV buffers (Iterator)", "[simple csv]") {
  csv::Reader csv;
  csv.read("inputs/test_01.csv");
  std::vector<csv::robin_map<std::string, std::string>> rows;
  while (csv.busy()) {
    if (csv.ready()) {
      auto row = csv.next_row();
      rows.push_back(row);
    }
  }

  REQUIRE(rows.size() == 2);
  REQUIRE(rows[0]["a"] == "1");
  REQUIRE(rows[0]["b"] == "2");
  REQUIRE(rows[0]["c"] == "3");
  REQUIRE(rows[1]["a"] == "4");
  REQUIRE(rows[1]["b"] == "5");
  REQUIRE(rows[1]["c"] == "6");
}

TEST_CASE("Parse the most basic of CSV buffers with ', ' delimiter", "[simple csv]") {
  csv::Reader csv;
  auto foo = csv.configure_dialect("test_dialect");
  csv.configure_dialect("test_dialect")
    .delimiter(", ");
  csv.read("inputs/test_02.csv");
  auto rows = csv.rows();
  REQUIRE(rows.size() == 2);
  REQUIRE(rows[0]["a"] == "1");
  REQUIRE(rows[0]["b"] == "2");
  REQUIRE(rows[0]["c"] == "3");
  REQUIRE(rows[1]["a"] == "4");
  REQUIRE(rows[1]["b"] == "5");
  REQUIRE(rows[1]["c"] == "6");
}

TEST_CASE("Parse the most basic of CSV buffers with ', ' delimiter using skip_initial_space_", "[simple csv]") {
  csv::Reader csv;
  csv.configure_dialect("test_dialect")
    .delimiter(", ")
    .skip_initial_space(true);
  csv.read("inputs/test_02.csv");

  auto rows = csv.rows();
  REQUIRE(rows.size() == 2);
  REQUIRE(rows[0]["a"] == "1");
  REQUIRE(rows[0]["b"] == "2");
  REQUIRE(rows[0]["c"] == "3");
  REQUIRE(rows[1]["a"] == "4");
  REQUIRE(rows[1]["b"] == "5");
  REQUIRE(rows[1]["c"] == "6");
}

TEST_CASE("Parse the most basic of CSV buffers with '::' delimiter", "[simple csv]") {
  csv::Reader csv;
  csv.configure_dialect("test_dialect")
    .delimiter("::");
  csv.read("inputs/test_03.csv");

  auto rows = csv.rows();
  REQUIRE(rows.size() == 2);
  REQUIRE(rows[0]["a"] == "1");
  REQUIRE(rows[0]["b"] == "2");
  REQUIRE(rows[0]["c"] == "3");
  REQUIRE(rows[1]["a"] == "4");
  REQUIRE(rows[1]["b"] == "5");
  REQUIRE(rows[1]["c"] == "6");
}

TEST_CASE("Parse the most basic of CSV buffers - Trim whitespace characters", "[simple csv]") {
  csv::Reader csv;

  csv.configure_dialect("test_dialect")
    .trim_characters(' ', '\t');
  csv.read("inputs/test_02.csv");
  auto rows = csv.rows();
  REQUIRE(rows.size() == 2);
  REQUIRE(rows[0]["a"] == "1");
  REQUIRE(rows[0]["b"] == "2");
  REQUIRE(rows[0]["c"] == "3");
  REQUIRE(rows[1]["a"] == "4");
  REQUIRE(rows[1]["b"] == "5");
  REQUIRE(rows[1]["c"] == "6");
}

TEST_CASE("Parse the most basic of CSV buffers - Trim whitespace characters gone crazy", "[simple csv]") {
  csv::Reader csv;
  csv.configure_dialect("test_dialect")
    .trim_characters(' ', '\t');
  csv.read("inputs/test_04.csv");
  auto rows = csv.rows();
  REQUIRE(rows.size() == 2);
  REQUIRE(rows[0]["a"] == "1");
  REQUIRE(rows[0]["b"] == "2");
  REQUIRE(rows[0]["c"] == "3");
  REQUIRE(rows[1]["a"] == "4");
  REQUIRE(rows[1]["b"] == "5");
  REQUIRE(rows[1]["c"] == "6");
}

TEST_CASE("Parse the most basic of CSV buffers - Log messages", "[simple csv]") {
  csv::Reader csv;
  csv.configure_dialect("test_dialect")
    .delimiter("::");
  csv.read("inputs/test_05.csv");
  auto rows = csv.rows();
  REQUIRE(rows.size() == 3);
  REQUIRE(rows[0]["Thread_ID"] == "1");
  REQUIRE(rows[0]["Log_Level"] == "DEBUG");
  REQUIRE(rows[0]["Message"] == "Thread Started");
  REQUIRE(rows[1]["Thread_ID"] == "2");
  REQUIRE(rows[1]["Log_Level"] == "DEBUG");
  REQUIRE(rows[1]["Message"] == "Thread Started");
  REQUIRE(rows[2]["Thread_ID"] == "3");
  REQUIRE(rows[2]["Log_Level"] == "ERROR");
  REQUIRE(rows[2]["Message"] == "File not found");
}

TEST_CASE("Parse the most basic of CSV buffers - No header row", "[simple csv]") {
  csv::Reader csv;

  csv.configure_dialect("test_dialect")
    .header(false);
  csv.read("inputs/test_08.csv");
  auto rows = csv.rows();
  REQUIRE(rows.size() == 3);
  REQUIRE(rows[0]["0"] == "1");
  REQUIRE(rows[0]["1"] == "2");
  REQUIRE(rows[0]["2"] == "3");
  REQUIRE(rows[1]["0"] == "4");
  REQUIRE(rows[1]["1"] == "5");
  REQUIRE(rows[1]["2"] == "6");
  REQUIRE(rows[2]["0"] == "7");
  REQUIRE(rows[2]["1"] == "8");
  REQUIRE(rows[2]["2"] == "9");
}

TEST_CASE("Parse the most basic of CSV buffers - Space delimiter", "[simple csv]") {
  csv::Reader csv;

  csv.configure_dialect("test_dialect")
    .delimiter(" ");
  csv.read("inputs/test_09.csv");
  auto rows = csv.rows();
  REQUIRE(rows.size() == 2);
  REQUIRE(rows[0]["first_name"] == "Eric");
  REQUIRE(rows[0]["last_name"] == "Idle");
  REQUIRE(rows[1]["first_name"] == "John");
  REQUIRE(rows[1]["last_name"] == "Cleese");
}

TEST_CASE("Parse the most basic of CSV buffers - Log with header", "[simple csv]") {
  csv::Reader csv;

  csv.configure_dialect("test_dialect")
    .delimiter("::")
    .trim_characters(' ', '[', ']');
  csv.read("inputs/test_10.csv");
  auto rows = csv.rows();
  REQUIRE(rows.size() == 2);
  REQUIRE(rows[0]["Timestamp"] == "1555164718");
  REQUIRE(rows[0]["Thread ID"] == "04");
  REQUIRE(rows[0]["Log Level"] == "INFO");
  REQUIRE(rows[0]["Log Message"] == "Hello World");
  REQUIRE(rows[1]["Timestamp"] == "1555463132");
  REQUIRE(rows[1]["Thread ID"] == "02");
  REQUIRE(rows[1]["Log Level"] == "DEBUG");
  REQUIRE(rows[1]["Log Message"] == "Warning! Foo has happened");
}

TEST_CASE("Parse Excel CSV", "[simple csv]") {
  csv::Reader csv;
  csv.use_dialect("excel");
  csv.read("inputs/test_11_excel.csv");
  auto rows = csv.rows();
  REQUIRE(rows.size() == 2);
  REQUIRE(rows[0]["a"] == "1");
  REQUIRE(rows[0]["b"] == "2");
  REQUIRE(rows[0]["c"] == "3");
  REQUIRE(rows[1]["a"] == "4");
  REQUIRE(rows[1]["b"] == "5");
  REQUIRE(rows[1]["c"] == "6");
}

TEST_CASE("Parse Unix CSV", "[simple csv]") {
  csv::Reader csv;
  csv.use_dialect("unix");
  csv.read("inputs/test_12_unix.csv");
  auto rows = csv.rows();
  REQUIRE(rows.size() == 2);
  REQUIRE(rows[0]["a"] == "1");
  REQUIRE(rows[0]["b"] == "2");
  REQUIRE(rows[0]["c"] == "3");
  REQUIRE(rows[1]["a"] == "4");
  REQUIRE(rows[1]["b"] == "5");
  REQUIRE(rows[1]["c"] == "6");
}

TEST_CASE("Parse messed up log files", "[simple csv]") {
  csv::Reader csv;

  csv.configure_dialect("my strange dialect")
    .delimiter("::")
    .trim_characters(' ', '[', ']', '{', '}');
  csv.read("inputs/test_13.csv");
  auto rows = csv.rows();
  REQUIRE(rows.size() == 2);
  REQUIRE(rows.size() == 2);
  REQUIRE(rows[0]["Timestamp"] == "1555164718");
  REQUIRE(rows[0]["Thread ID"] == "04");
  REQUIRE(rows[0]["Log Level"] == "INFO");
  REQUIRE(rows[0]["Log Message"] == "Hello World");
  REQUIRE(rows[1]["Timestamp"] == "1555463132");
  REQUIRE(rows[1]["Thread ID"] == "02");
  REQUIRE(rows[1]["Log Level"] == "DEBUG");
  REQUIRE(rows[1]["Log Message"] == "Warning! Foo has happened");
}

TEST_CASE("Parse headers with double quotes", "[simple csv]") {
  csv::Reader csv;
  csv.read("inputs/test_06.csv");
  auto rows = csv.rows();
  REQUIRE(rows.size() == 0);
  auto cols = csv.cols();
  REQUIRE(cols.size() == 3);
  REQUIRE(cols[0] == "\"Free trip to A,B\"");
  REQUIRE(cols[1] == "\"5.89\"");
  REQUIRE(cols[2] == "\"Special rate \"\"1.79\"\"\"");
}

TEST_CASE("Parse headers with pairs of single-quotes", "[simple csv]") {
  csv::Reader csv;
  csv.configure_dialect("test_dialect")
    .quote_character('\'');
  csv.read("inputs/test_07.csv");
  auto rows = csv.rows();
  REQUIRE(rows.size() == 0);
  auto cols = csv.cols();
  REQUIRE(cols.size() == 3);
  REQUIRE(cols[0] == "''Free trip to A,B''");
  REQUIRE(cols[1] == "''5.89''");
  REQUIRE(cols[2] == "''Special rate ''''1.79''''''");
}

TEST_CASE("Parse the most basic of CSV buffers and ignore 1 column", "[simple csv]") {
  csv::Reader csv;
  csv.configure_dialect("test_dialect")
    .ignore_columns("a");
  csv.read("inputs/test_01.csv");
  auto rows = csv.rows();
  REQUIRE(rows.size() == 2);
  REQUIRE(rows[0].count("a") == 0);
  REQUIRE(rows[0]["b"] == "2");
  REQUIRE(rows[0]["c"] == "3");
  REQUIRE(rows[1].count("a") == 0);
  REQUIRE(rows[1]["b"] == "5");
  REQUIRE(rows[1]["c"] == "6");
}

TEST_CASE("Parse the most basic of CSV buffers and ignore 2 columns", "[simple csv]") {
  csv::Reader csv;
  csv.configure_dialect("test_dialect")
    .ignore_columns("a", "b");
  csv.read("inputs/test_01.csv");
  auto rows = csv.rows();
  REQUIRE(rows.size() == 2);
  REQUIRE(rows[0].count("a") == 0);
  REQUIRE(rows[0].count("b") == 0);
  REQUIRE(rows[0]["c"] == "3");
  REQUIRE(rows[1].count("a") == 0);
  REQUIRE(rows[1].count("b") == 0);
  REQUIRE(rows[1]["c"] == "6");
}

TEST_CASE("Parse the most basic of CSV buffers and ignore all columns", "[simple csv]") {
  csv::Reader csv;
  csv.configure_dialect("test_dialect")
    .ignore_columns("a", "b", "c");
  csv.read("inputs/test_01.csv");
  auto rows = csv.rows();
  REQUIRE(rows.size() == 2);
  REQUIRE(rows[0].size() == 0);
  REQUIRE(rows[1].size() == 0);
}

TEST_CASE("Parse the most basic of CSV buffers and ignore age/gender columns", "[simple csv]") {
  csv::Reader csv;
  csv.configure_dialect("test_dialect")
    .delimiter(", ")
    .ignore_columns("age", "gender");
  csv.read("inputs/test_14.csv");
  auto rows = csv.rows();
  REQUIRE(rows.size() == 3);
  REQUIRE(rows[0]["name"] == "Mark Johnson");
  REQUIRE(rows[0]["email"] == "mark.johnson@gmail.com");
  REQUIRE(rows[0]["department"] == "BA");
  REQUIRE(rows[0].count("age") == 0);
  REQUIRE(rows[0].count("gender") == 0);
  REQUIRE(rows[1]["name"] == "John Stevenson");
  REQUIRE(rows[1]["email"] == "john.stevenson@gmail.com");
  REQUIRE(rows[1]["department"] == "IT");
  REQUIRE(rows[1].count("age") == 0);
  REQUIRE(rows[1].count("gender") == 0);
  REQUIRE(rows[2]["name"] == "Jane Barkley");
  REQUIRE(rows[2]["email"] == "jane.barkley@gmail.com");
  REQUIRE(rows[2]["department"] == "MGT");
  REQUIRE(rows[2].count("age") == 0);
  REQUIRE(rows[2].count("gender") == 0);
}