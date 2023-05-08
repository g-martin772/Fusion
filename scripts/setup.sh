#!/bin/bash

PYTHON_VERSION=
PYTHON_FILE=setup.py

echo "Checking if Python is installed..."

if command -v python3 &>/dev/null; then
    echo "Python is installed. Checking for Python version $PYTHON_VERSION..."
    if [[ $(python3 --version 2>&1) == *"$PYTHON_VERSION"* ]]; then
        echo "Python version $PYTHON_VERSION found. Running $PYTHON_FILE..."
        python3 $PYTHON_FILE
    else
        echo "Python version $PYTHON_VERSION not found. Installing Python version $PYTHON_VERSION..."
        sudo apt update
        sudo apt install -y python$PYTHON_VERSION
        if [[ $(python3 --version 2>&1) == *"$PYTHON_VERSION"* ]]; then
            echo "Python version $PYTHON_VERSION installed successfully. Running $PYTHON_FILE..."
            python3 $PYTHON_FILE
        else
            echo "Failed to install Python version $PYTHON_VERSION. Exiting..."
            exit 1
        fi
    fi
else
    echo "Python is not installed. Installing Python version $PYTHON_VERSION..."
    sudo apt update
    sudo apt install -y python$PYTHON_VERSION
    if command -v python3 &>/dev/null; then
        echo "Python version $PYTHON_VERSION installed successfully. Running $PYTHON_FILE..."
        python3 $PYTHON_FILE
    else
        echo "Failed to install Python version $PYTHON_VERSION. Exiting..."
        exit 1
    fi
fi
