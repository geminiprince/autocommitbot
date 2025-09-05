#!/bin/bash

# Configuration
REPO_DIR="/tmp/dailycommit"
GIT_EMAIL="geminiprinceee@gmail.com"
GIT_NAME="geminiprince"
REPO_URL="https://github.com/geminiprince/dailycommit.git"

# Array of quotes
QUOTES=(
    "The only way to do great work is to love what you do. - Steve Jobs"
    "Innovation distinguishes between a leader and a follower. - Steve Jobs"
    "The future belongs to those who believe in the beauty of their dreams. - Eleanor Roosevelt"
    "Success is not final, failure is not fatal: It is the courage to continue that counts. - Winston Churchill"
    "The only limit to our realization of tomorrow will be our doubts of today. - Franklin D. Roosevelt"
    "The way to get started is to quit talking and begin doing. - Walt Disney"
    "Your time is limited, don't waste it living someone else's life. - Steve Jobs"
    "The best way to predict the future is to invent it. - Alan Kay"
    "Stay hungry, stay foolish. - Steve Jobs"
    "Simplicity is the ultimate sophistication. - Leonardo da Vinci"
)

# Create or update repository
if [ ! -d "$REPO_DIR" ]; then
    git clone "$REPO_URL" "$REPO_DIR" || { echo "Failed to clone repository"; exit 1; }
    cd "$REPO_DIR" || exit 1
else
    cd "$REPO_DIR" || exit 1
    git pull origin main || { echo "Failed to pull latest changes"; exit 1; }
fi

# Set git config for this repository
git config user.email "$GIT_EMAIL"
git config user.name "$GIT_NAME"

# Generate a random quote
RANDOM_QUOTE="${QUOTES[$((RANDOM % ${#QUOTES[@]}))]}"

# Create a new file with the quote
TIMESTAMP=$(date +"%Y-%m-%d_%H-%M-%S")
FILENAME="quote_${TIMESTAMP}.txt"
echo "$RANDOM_QUOTE" > "$FILENAME"

# Add and commit the file
git add "$FILENAME"
git commit -m "Add new quote: ${RANDOM_QUOTE:0:30}..."

# Push changes
git push origin main
