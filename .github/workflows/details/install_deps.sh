source "$(dirname "$0")/../project_setup.sh"

echo Updating available packages
sudo apt-get update

for dep in "${PROJECT_DEPS[@]}"; do
    echo "Installing $dep"
    sudo apt-get install $dep
    echo "$dep has installed"
done
