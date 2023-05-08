@echo off

set PYTHON_VERSION=3.10
set PYTHON_FILE=setup.py

echo Checking if Python is installed...

where python >nul 2>&1

if %ERRORLEVEL%==0 (
    echo Python is installed. Checking for Python version %PYTHON_VERSION%...
    python --version | findstr /C:"Python %PYTHON_VERSION%" >nul 2>&1
    if %ERRORLEVEL%==0 (
        echo Python version %PYTHON_VERSION% found. Running %PYTHON_FILE%...
        python %PYTHON_FILE%
    ) else (
        echo Python version %PYTHON_VERSION% not found. Installing.
	winget install --exact --silent --override --id Python.Python --version %PYTHON_VERSION%.0
    )
) else (
    echo Python is not installed. Installing.
    winget install --exact --silent --override --id Python.Python --version %PYTHON_VERSION%.0
)

PAUSE