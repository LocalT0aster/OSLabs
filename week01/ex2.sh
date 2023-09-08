sudo apt-get install sl cowsay fortune lolcat
cowsay Hello World!
cowsay Hello World! | lolcat
fortune | cowsay | lolcat
sl | lolcat
sudo nano /usr/share/cowsay/cows/boykisser.cow
fortune | cowsay -f boykisser | lolcat
sudo touch /usr/share/games/fortunes/boykisser_quotes
code /usr/share/games/fortunes/boykisser_quotes
cd /usr/share/games/fortunes
sudo strfile boykisser_quotes
fortune boykisser_quotes | cowsay -f boykisser | lolcat
cd ~
echo "fortune | cowsay -f boykisser | lolcat" >> .bashrc
cd ~/Repos/OSLabs/week01
history > ex2.txt
cat ex2.txt
cat ex2.txt | cut -c 8- > ex2.sh
cat ex2.sh
nano ex2.sh
