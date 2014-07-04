Feature: program runs showcase

  In order to run fast code experiments
  As a developer of the code
  I want a mode to show the showcase

	Scenario: showcase runs
    When I start with --testrun=showcase
    Then I should see "testing marian: is matching."

