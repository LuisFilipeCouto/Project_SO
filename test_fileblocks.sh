rm -f grpfile
touch grpfile
fileblock=0
source scripts/sofs20.sh
c
f
inode=1
echo -ne "ai\n1\n644\nq\n" | tt
while [[ $fileblock -le 20 ]] ; do
   echo -ne "afb\n$inode\n$fileblock\nq\n" | tt
   s -i 1 >> grpfile
  (( fileblock += 1 ))
done
echo -ne "ffb\n$inode\n0\nq\n" | tt
s -i 1 >> grpfile

rm -f binfile
touch binfile
cd sofs20-bin
fileblock=0
source scripts/sofs20.sh
c
f
echo -ne "ai\n1\n644\nq\n" | tt
while [[ $fileblock -le 20 ]] ; do
   echo -ne "afb\n$inode\n$fileblock\nq\n" | tt
   s -i 1 >> ../binfile
  (( fileblock += 1 ))
done
echo -ne "ffb\n$inode\n0\nq\n" | tt
s -i 1 >> ../binfile
