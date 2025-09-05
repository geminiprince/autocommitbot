#!/bin/bash

# Configuration
REPO_DIR="$HOME/dailycommit"
GIT_EMAIL="geminiprinceee@gmail.com"
GIT_NAME="geminiprince"
REPO_URL="https://github.com/geminiprince/dailycommit.git"
COMMIT_INTERVAL=300  # 5 minutes

# Array of quotes
QUOTES=(
    "The only way to do great work is to love what you do. - Steve Jobs"
    "Innovation distinguishes between a leader and a follower. - Steve Jobs"
    "The future belongs to those who believe in the beauty of their dreams. - Eleanor Roosevelt"
    
    # Add and commit the file
    echo "Committing changes..."
    git add "$filename"
    git commit -m "Add new quote: ${quote:0:30}..."
    
    # Push changes using GitHub CLI for authentication
    echo "Pushing changes..."
    git push origin main
}

# Main function
main() {
    echo "Starting auto-commit bot..."
    echo "Repository: $REPO_DIR"
    echo "Commit interval: $COMMIT_INTERVAL seconds"
    echo "Press Ctrl+C to stop"
    echo ""
    
    # Initialize the repository
    init_repo
    
    # Main loop
    while true; do
        echo "[$(date '+%Y-%m-%d %H:%M:%S')] Checking for changes..."
        create_commit
        echo "Waiting $COMMIT_INTERVAL seconds until next commit..."
        echo ""
        sleep "$COMMIT_INTERVAL"
    done
}

# Run the script
main
