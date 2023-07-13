rm -f grpfile
touch grpfile
i=10
while [[ $i -le 100 ]] ; do
   source scripts/sofs20.sh
   c $i
   f
   s >> grpfile
  (( i += 1 ))
done

rm -f binfile
touch binfile
cd sofs20-bin
i=10
while [[ $i -le 100 ]] ; do
   source scripts/sofs20.sh
   c $i
   f
   s >> ../binfile
  (( i += 1 ))
done

