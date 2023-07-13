rm -f grpfile
touch grpfile
direntry=0
source scripts/sofs20.sh
c
f
inode=1
echo -ne "ai\n1\n644\nq\n" | tt
while [[ $direntry -le 40 ]] ; do
   echo -ne "ade\n0\n$direntry\n$inode\nq\n" | tt
   s -i 1 >> grpfile
   s -d 5 >> grpfile
   echo "-----------">>grpfile
   s -d 6 >> grpfile
  (( direntry += 1 ))
done

direntry=20
while [[ $direntry -ge 2 ]] ; do
   echo -ne "dde\n0\n$direntry\nq\n" | tt
   s -i 1 >> grpfile
   s -d 5 >> grpfile
   echo "-----------">>grpfile
   s -d 6 >> grpfile
  (( direntry -= 1 ))
done

rm -f binfile
touch binfile
cd sofs20-bin
direntry=0
source scripts/sofs20.sh
c
f
echo -ne "ai\n1\n644\nq\n" | tt
while [[ $direntry -le 40 ]] ; do
   echo -ne "ade\n0\n$direntry\n$inode\nq\n" | tt
   s -i 1 >> ../binfile
   s -d 5 >> ../binfile
   echo "-----------">>../binfile
   s -d 6 >> ../binfile
  (( direntry += 1 ))
done

direntry=20
while [[ $direntry -ge 2 ]] ; do
   echo -ne "dde\n0\n$direntry\nq\n" | tt
   s -i 1 >> ../binfile
   s -d 5 >> ../binfile
   echo "-----------">>../binfile
   s -d 6 >> ../binfile
  (( direntry -= 1 ))
done