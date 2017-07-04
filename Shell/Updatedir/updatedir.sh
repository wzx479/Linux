read -p 'type the marster directory of source' SOURCEDIR
if[ ! -d "$SOURCEDIR"];then
	echo "no such dir"
	exit 1
fi

read -p 'type the marster directory of destination' DESTDIR
if[ ! -d "$DESTDIR"];then
	echo "no such dir"
	exit 1
fi

declare -i counter = 0
function ReplaceOrCreate()
{
	if[ ! -d "$[DESTDIR]/$1"];then
		mkdir "$[DESTDIR]/$1"
		for file in $(ls -l $1|awk '/^-.+/ {print $NF}')
		do 
			cp "${1}/$file" "$DESTDIR/${1}/$file"
			counter = $[counter +1]
		done
	else 
		for file in $(ls -l $1|awk '/^-.+/ {print $NF}')
		do 
			if[$(find "${DESTDIR}/$1" -name "$file"|wc -l) -eq 1];then
			
				mv "${DESTDIR}/${1}/$file" "${DESTDIR}/${1}/${file}.${date + %Y-%m-%d-%H-%M-%S}"		
				cp "${1}/$file" "${DESTDIR}/${1}/$file"
			else
			    cp "${1}/$file" "${DESTDIR}/${1}/$file"
			fi
			
			counter = $[counter +1]
		done
		
	for directory in $(ls -l $1|awk '/^d.+/ {print $NF}')
		do 
			ReplaceOrCreate"${1}/$directory"
		done
}


cd "$SOURCEDIR"
for directory in $(ls -l | awk '/^d./{print $NF}')
do

			ReplaceOrCreate"$directory"

done 
do 
	if [$(find "$DESTDIR" -name "$file" |wc -l) -eq 1];then
		mv "{DESTDIR}/${file}" "${DESTDIR}/${file}.${date + %Y-%m-%d-%H-%M-%S}"	
		cp "$file" "${DESTDIR}/${file}"
	else
		cp "$file" "${DESTDIR}/${file}"
	fi
		counter = $[counter +1]
done

echo "$counter files have been replaced"		
		
		
			
		
		
		
		
		