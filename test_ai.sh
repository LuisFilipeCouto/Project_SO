rm -f grpfile
touch grpfile
i=0
source scripts/sofs20.sh
c
f
while [[ $i -le 50 ]] ; do
   echo -ne "ai\n1\n644\nq\n" | tt
   s >> grpfile
  (( i += 1 ))
done
j=0
while [[ $j -le 20 ]] ; do
   echo -ne "fi\n$j\nq\n" | tt
   s >> grpfile
  (( j += 1 ))
done




rm -f binfile
touch binfile
cd sofs20-bin
i=0
source scripts/sofs20.sh
c
f
while [[ $i -le 50 ]] ; do
   echo -ne "ai\n1\n644\nq\n" | tt
   s >> ../binfile
  (( i += 1 ))
done
j=0
while [[ $j -le 20 ]] ; do
   echo -ne "fi\n$j\nq\n" | tt
   s >> ../binfile
  (( j += 1 ))
done
