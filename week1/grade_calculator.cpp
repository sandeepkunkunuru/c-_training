#include <iostream>
#include <string>
#include <iomanip>  // For formatting output

int main() {
    // Variables to store student information and scores
    std::string student_name;
    double assignment_score, midterm_score, final_exam_score;
    double total_score;
    char letter_grade;
    
    // Display welcome message
    std::cout << "===== Student Grade Calculator =====" << std::endl;
    
    // Get student information
    std::cout << "Enter student name: ";
    std::getline(std::cin, student_name);
    
    // Get scores
    std::cout << "Enter assignment score (0-100): ";
    std::cin >> assignment_score;
    
    std::cout << "Enter midterm exam score (0-100): ";
    std::cin >> midterm_score;
    
    std::cout << "Enter final exam score (0-100): ";
    std::cin >> final_exam_score;
    
    // Validate input scores
    if (assignment_score < 0 || assignment_score > 100 ||
        midterm_score < 0 || midterm_score > 100 ||
        final_exam_score < 0 || final_exam_score > 100) {
        std::cout << "Error: All scores must be between 0 and 100." << std::endl;
        return 1;  // Exit with error code
    }
    
    // Calculate total score (assignments: 30%, midterm: 30%, final: 40%)
    total_score = (assignment_score * 0.3) + (midterm_score * 0.3) + (final_exam_score * 0.4);
    
    // Determine letter grade using if-else statements
    if (total_score >= 90) {
        letter_grade = 'A';
    } else if (total_score >= 80) {
        letter_grade = 'B';
    } else if (total_score >= 70) {
        letter_grade = 'C';
    } else if (total_score >= 60) {
        letter_grade = 'D';
    } else {
        letter_grade = 'F';
    }
    
    // Display results
    std::cout << "\n===== Grade Report =====" << std::endl;
    std::cout << "Student: " << student_name << std::endl;
    std::cout << std::fixed << std::setprecision(1);  // Set decimal precision
    std::cout << "Assignment Score: " << assignment_score << " / 100" << std::endl;
    std::cout << "Midterm Exam Score: " << midterm_score << " / 100" << std::endl;
    std::cout << "Final Exam Score: " << final_exam_score << " / 100" << std::endl;
    std::cout << "Total Score: " << total_score << " / 100" << std::endl;
    std::cout << "Letter Grade: " << letter_grade << std::endl;
    
    // Provide feedback based on the letter grade
    std::cout << "\nFeedback: ";
    switch (letter_grade) {
        case 'A':
            std::cout << "Excellent work! Keep it up!" << std::endl;
            break;
        case 'B':
            std::cout << "Good job! You're doing well." << std::endl;
            break;
        case 'C':
            std::cout << "Satisfactory. Consider more practice to improve." << std::endl;
            break;
        case 'D':
            std::cout << "You passed, but need significant improvement." << std::endl;
            break;
        case 'F':
            std::cout << "Unfortunately, you did not pass. Please seek additional help." << std::endl;
            break;
    }
    
    // Additional information using ternary operator
    std::string status = (total_score >= 60) ? "Passed" : "Failed";
    std::cout << "Course Status: " << status << std::endl;
    
    return 0;
}
