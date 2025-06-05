#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

void clearConsole() {
    system(CLEAR);
}

int innings = 0;
int target = 0;
int userScore = 0;
int computerScore = 0;
bool isUserBatFirst = false;

int handleToss(char user_choice);
void askBatorBowl();
void playInnings(bool isUserBatting);
int iswicket(int batchoice, int ballchoice);
int win(int target, int totalruns);
void matchSummary();

int main() {
    clearConsole();
    srand(time(0));

    printf("Welcome to Hand Cricket!\n");
    printf("Let's begin with the toss.\n");

    char user_choice;
    int valid = 0;

    while (!valid) {
        printf("Choose Heads (H) or Tails (T): ");
        scanf(" %c", &user_choice);
        valid = handleToss(user_choice);
    }

    return 0;
}

int handleToss(char user_choice) {
    if (user_choice != 'H' && user_choice != 'h' && user_choice != 'T' && user_choice != 't') {
        printf("❌ Invalid choice! Please choose either H or T.\n");
        return 0;
    }

    int toss = rand() % 2;
    printf("Tossing the coin...\n");
    printf("🪙 Result: %s\n", toss == 0 ? "Heads" : "Tails");

    int user_pick = (user_choice == 'H' || user_choice == 'h') ? 0 : 1;

    if (user_pick == toss) {
        printf("🎉 You won the toss!\n");
        askBatorBowl();
    } else {
        printf("🤖 You lost the toss. Computer will decide...\n");
        int computer_choice = rand() % 2;
        printf("Computer chose to %s.\n", computer_choice == 0 ? "Bat" : "Bowl");
        isUserBatFirst = (computer_choice == 1);
        playInnings(isUserBatFirst);
        innings++;
        playInnings(!isUserBatFirst);
    }

    return 1;
}

void askBatorBowl() {
    char choice;
    printf("Do you want to Bat or Bowl? (B for Bat, L for Bowl): ");
    scanf(" %c", &choice);

    if (choice == 'B' || choice == 'b') {
        isUserBatFirst = true;
        printf("🏏 You chose to Bat!\n");
        playInnings(true);
        innings++;
        playInnings(false);
    } else if (choice == 'L' || choice == 'l') {
        isUserBatFirst = false;
        printf("🎯 You chose to Bowl!\n");
        playInnings(false);
        innings++;
        playInnings(true);
    } else {
        printf("❌ Invalid choice! Please choose either B or L.\n");
        askBatorBowl();
    }
}

void playInnings(bool isUserBatting) {
    printf("\n--- %s's Innings ---\n", isUserBatting ? "Your" : "Computer's");

    int shot, ball, balls = 6, totalruns = 0;

    for (int i = 0; i < balls; ++i) {
        if (isUserBatting) {
            printf("Enter your shot (1-6): ");
            scanf("%d", &shot);
            if (shot < 1 || shot > 6) {
                printf("❌ Invalid shot! Try again.\n");
                i--;
                continue;
            }
            ball = rand() % 6 + 1;
            printf("You chose: %d, Ball: %d\n", shot, ball);
        } else {
            printf("Enter your ball (1-6): ");
            scanf("%d", &ball);
            if (ball < 1 || ball > 6) {
                printf("❌ Invalid ball! Try again.\n");
                i--;
                continue;
            }
            shot = rand() % 6 + 1;
            printf("You chose: %d, Shot: %d\n", ball, shot);
        }

        if (iswicket(shot, ball)) {
            printf("🚨 Wicket! %s is out!\n", isUserBatting ? "You" : "Computer");
            break;
        } else {
            totalruns += shot;
            printf("Total Runs: %d\n", totalruns);
        }

        // FIXED: Update score based on current batting player, not who batted first
        if (isUserBatting) {
            userScore = totalruns;
        } else {
            computerScore = totalruns;
        }

        if (innings == 1) {
            int result = win(target, totalruns);
            // FIXED: win() never returns 0, so removed result == 0 condition
            if (result == 1 || result == 2) {
                matchSummary();
                return;
            }
        }

        printf("Balls left: %d\n", balls - i - 1);
    }

    if (innings == 0) {
        target = totalruns;
        printf("Target set: %d runs.\n", target);
    } 
    // FIXED: Removed invalid parameterized matchSummary call here, call without params
    else {
        matchSummary();
    }
}

int iswicket(int batchoice, int ballchoice) {
    return batchoice == ballchoice;
}

int win(int target, int totalruns) {
    if (totalruns > target) return 1;  // Win
    if (totalruns == target) return 2; // Draw
    return -1;                         // Not over yet or loss
}

void matchSummary() {
    printf("\n🏁 Match Summary 🏁\n");
    printf("Your Score     : %d\n", userScore);
    printf("Computer Score : %d\n", computerScore);

    if (userScore == computerScore) {
        printf("🤝 Match Drawn!\n");
    } 
    // FIXED: corrected variable name from userBatFirst to isUserBatFirst
    else if ((isUserBatFirst && userScore > computerScore) || (!isUserBatFirst && computerScore > userScore)) {
        printf("🎉 You won the match!\n");
    } else {
        printf("💔 You lost the match!\n");
    }
}
