#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>
#include <limits>
#include <random>

void load();
void questionHandlingFunction();

std::random_device rd;
std::mt19937 gen(rd());

int numberOfQuestions;
int first;
int second;
int third;
int fourth;

std::vector<std::string> questions;
std::vector<std::string> answer1;
std::vector<std::string> answer2;
std::vector<std::string> answer3;
std::vector<std::string> answer4;
std::vector<int> correctAnswer;
std::vector<int> points;

void questionHandlingFunction() {
	load();
}

void load() {

	questions.clear();
	answer1.clear();
	answer2.clear();
	answer3.clear();
	answer4.clear();
	correctAnswer.clear();
	points.clear();

	int a;
	int b;
	int c;
	int d;

	std::string line;

	std::ifstream file("questions.txt");

	if (!file.is_open()) {
		return;
	}

	file >> numberOfQuestions;
	file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	for (int v = 0; v < numberOfQuestions; v++) {
		std::getline(file, line);

		std::stringstream ss(line);

		int point;

		std::string question;
		std::string firstAnswer;
		std::string secondAnswer;
		std::string thirdAnswer;
		std::string fourthAnswer;

		std::getline(ss, question, '|');
		std::getline(ss, firstAnswer, '|');
		std::getline(ss, secondAnswer, '|');
		std::getline(ss, thirdAnswer, '|');
		std::getline(ss, fourthAnswer, '|');
		ss >> point;

		questions.push_back(question);

		std::vector<int> positions = { 1, 2, 3, 4 };
		std::shuffle(positions.begin(), positions.end(), gen);

		a = positions[0];
		b = positions[1];
		c = positions[2];
		d = positions[3];

		if (a == 1) {
			answer1.push_back(firstAnswer);
			correctAnswer.push_back(1);
		}
		else if (a == 2) {
			answer2.push_back(firstAnswer);
			correctAnswer.push_back(2);
		}
		else if (a == 3) {
			answer3.push_back(firstAnswer);
			correctAnswer.push_back(3);
		}
		else if (a == 4) {
			answer4.push_back(firstAnswer);
			correctAnswer.push_back(4);
		}
		if (b == 1) {
			answer1.push_back(secondAnswer);
		}
		else if (b == 2) {
			answer2.push_back(secondAnswer);
		}
		else if (b == 3) {
			answer3.push_back(secondAnswer);
		}
		else if (b == 4) {
			answer4.push_back(secondAnswer);
		}
		if (c == 1) {
			answer1.push_back(thirdAnswer);
		}
		else if (c == 2) {
			answer2.push_back(thirdAnswer);
		}
		else if (c == 3) {
			answer3.push_back(thirdAnswer);
		}
		else if (c == 4) {
			answer4.push_back(thirdAnswer);
		}
		if (d == 1) {
			answer1.push_back(fourthAnswer);
		}
		else if (d == 2) {
			answer2.push_back(fourthAnswer);
		}
		else if (d == 3) {
			answer3.push_back(fourthAnswer);
		}
		else if (d == 4) {
			answer4.push_back(fourthAnswer);
		}

		points.push_back(point);

	}

}
