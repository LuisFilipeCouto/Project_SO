rm -f grpfile
touch grpfile
i=0
source scripts/sofs20.sh
c
f
while [[ $i -le 800 ]] ; do
   echo -ne "adb\nq\n" | tt
   s >> grpfile
  (( i += 1 ))
done
j=0
while [[ $j -le 100 ]] ; do
   echo -ne "fdb\n$j\nq\n" | tt
   s >> grpfile
  (( j += 1 ))
done

k=0
while [[ $k -le 200 ]] ; do
   echo -ne "adb\nq\n" | tt
   s >> grpfile
  (( k += 1 ))
done


rm -f binfile
touch binfile
cd sofs20-bin
i=0
source scripts/sofs20.sh
c
f
while [[ $i -le 800 ]] ; do
   echo -ne "adb\nq\n" | tt
   s >> ../binfile
  (( i += 1 ))
done
j=0
while [[ $j -le 100 ]] ; do
   echo -ne "fdb\n$j\nq\n" | tt
   s >> ../binfile
  (( j += 1 ))
done

k=0
while [[ $k -le 200 ]] ; do
   echo -ne "adb\nq\n" | tt
   s >> ../binfile
  (( k += 1 ))
done