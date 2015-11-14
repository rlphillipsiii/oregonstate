numbers=(10 100 1000 10000 100000 1000000 10000000 100000000 1000000000, 4294967295);

jobs=(10 20 30 40 50 60 70);

for job in "${jobs[@]}"
do
    for num in "${numbers[@]}"
    do
        echo -e "\nThreads:  max = $num, jobs = $job";
	./primes t $num $job
    done
done

for job in "${jobs[@]}"
do
    for num in "${numbers[@]}"
    do
	echo -e "\nProcesses:  max = $num, jobs = $job";
	./primes p $num $job
    done
done