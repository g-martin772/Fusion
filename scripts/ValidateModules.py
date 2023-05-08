import sys
import subprocess
import importlib.util as importlib_util


def Validate():
    for packageName in ["requests"]:
        if not ValidatePackage(packageName):
            return # cannot validate further


def ValidatePackage(packageName):
    if importlib_util.find_spec(packageName) is None:
        return InstallPackage(packageName)
    return True

def InstallPackage(packageName):
    permissionGranted = False
    while not permissionGranted:
        reply = str(input("Would you like to install Python package '{0:s}'? [Y/N]: ".format(packageName))).lower().strip()[:1]
        if reply == 'n':
            return False
        permissionGranted = (reply == 'y')
    
    print(f"Installing {packageName} module...")
    subprocess.check_call(['python', '-m', 'pip', 'install', packageName])

    return ValidatePackage(packageName)

if __name__ == "__main__":
    PythonConfiguration.Validate()