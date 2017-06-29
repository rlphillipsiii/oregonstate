
public class Sorting {

	private static void swap(Person[] a, int i, int j) {
		Person temp = a[i];
		a[i] = a[j];
		a[j] = temp;
	}

	public static void selectionSort(Person[] list) {
		for (int i = 0; i < list.length - 1; i++) {
			// Find the index of the minimum value
			int minPos = i;
			for (int j = i + 1; j < list.length; j++) {
				if (list[0].compare(list[j],  list[minPos]) == -1) {
					minPos = j;
				}
			}
			swap(list, minPos, i);
		}
	}

	public static void bubbleSort(Person[] list) {
		for (int i = (list.length - 1); i >= 0; i--) {
			for (int j = 1; j <= i; j++) {
				if (list[0].compare(list[j - 1], list[j]) == 1) {
					// swap elements at j-1 and j
					Person temp = list[j - 1];
					list[j - 1] = list[j];
					list[j] = temp;
				}
			}
		}
	}

	public static void insertionSort(Person[] list) {
		for (int i = 1; i < list.length; i++) {
			Person next = list[i];
			// find the insertion location while moving all larger element up
			int j = i;
			while (j > 0 && list[0].compare(list[j - 1] , next) == 1){
				list[j] = list[j - 1];
				j--;
			}
			// insert the element
			list[j] = next;
		}
	}

	public static void mergeSort(Person [] list) {
		if (list.length <= 1) {
			return;
		}

		// Split the array in half
		Person[] first = new Person[list.length / 2];
		Person[] second = new Person[list.length - first.length];
		System.arraycopy(list, 0, first, 0, first.length);
		System.arraycopy(list, first.length, second, 0, second.length);
		
		// Sort each half
		mergeSort(first);
		mergeSort(second);

		// Merge the halves together, overwriting the original array
		merge(first, second, list);
	}

	private static void merge(Person[] first, Person[] second, Person [] result) {
		// Merge both halves into the result array
		// Next element to consider in the first array
		int iFirst = 0;
		// Next element to consider in the second array
		int iSecond = 0;

		// Next open position in the result
		int j = 0;
		// As long as neither iFirst nor iSecond is past the end, move the
		// smaller element into the result.
		while (iFirst < first.length && iSecond < second.length) {
			if (first[0].compare(first[iFirst], second[iSecond]) == -1) {
				result[j] = first[iFirst];
				iFirst++;
			} else {
				result[j] = second[iSecond];
				iSecond++;
			}
			j++;
		}
		// copy what's left
		System.arraycopy(first, iFirst, result, j, first.length - iFirst);
		System.arraycopy(second, iSecond, result, j, second.length - iSecond);
	}

	// "from" and "to" specify the range of the array to sort
	private static void quicksort(Person list[], int from, int to) {
		// If the indexes cross, then we've sorted the whole array.
		if (from >= to) {
			return;
		}

		// Choose a pivot value and then partition the array so that every value
		// less than the pivot is positioned before the pivot in the array and
		// every value greater than the pivot is positioned after the pivot in
		// the array.
		Person pivot = list[from];
		int i = from - 1;
		int j = to + 1;
		while (i < j) {
			// Keep incrementing from the start of the range so long as the
			// values are less than the pivot.
			i++;
			while (list[0].compare(list[i], pivot) == -1) { i++; }
			// Keep decrementing from the end of the range so long as the values
			// are greater than the pivot.
			j--;
			while (list[0].compare(list[j], pivot) == 1) { j--; }
			// So long at the indexes have not crossed, swap the pivot with the
			// value that was out of place.
			if (i < j) {
				swap(list, i, j);
			}
		}

		// Recursively sort the two portions of the array
		quicksort(list, from, j);
		quicksort(list, j + 1, to);
	}

	// Helper method that kicks off the recursive quicksort method
	public static void quicksort(Person [] list) {
		quicksort(list, 0, list.length-1);
	}
	
//	public void sortTest(){
//		int[] ten = new int[10];
//		int[] hundred = new int [100];
//		int[] thousand = new int [1000];
//		int[] ten4 = new int [10000];
//		int[] ten5 = new int [100000];
//		Random rand = new Random(1000);
//		
//		for(int i = 0; i < ten.length; i++){
//			ten[i] = rand.nextInt(1000);
//		}
//		
//		System.out.println("");
//		System.out.println("int[10]");
//		System.out.println("Current Time:  " + System.currentTimeMillis());
//		
//		selectionSort(ten);
//		System.out.println("Selection Sort End:  " + System.currentTimeMillis());
//		
//		bubbleSort(ten);
//		System.out.println("Bubble Sort End:  " + System.currentTimeMillis());
//		
//		insertionSort(ten);
//		System.out.println("Insertion Sort End:  " + System.currentTimeMillis());
//		
//		mergeSort(ten);
//		System.out.println("Merge Sort End:  " + System.currentTimeMillis());
//		
//		quicksort(ten);
//		System.out.println("Sort Test End:  " + System.currentTimeMillis());
//		
//		for(int i = 0; i < hundred.length; i++){
//			hundred[i] = rand.nextInt(1000);
//		}
//		
//		System.out.println("");
//		System.out.println("int[100]");
//		
//		System.out.println("Start Time:  " + System.currentTimeMillis());
//		selectionSort(hundred);
//		System.out.println("Selection Sort End:  " + System.currentTimeMillis());
//		
//		System.out.println("Start Time:  " + System.currentTimeMillis());
//		bubbleSort(hundred);
//		System.out.println("Bubble Sort End:  " + System.currentTimeMillis());
//		
//		System.out.println("Start Time:  " + System.currentTimeMillis());
//		insertionSort(hundred);
//		System.out.println("Insertion Sort End:  " + System.currentTimeMillis());
//		
//		System.out.println("Start Time:  " + System.currentTimeMillis());
//		mergeSort(hundred);
//		System.out.println("Merge Sort End:  " + System.currentTimeMillis());
//		
//		System.out.println("Start Time:  " + System.currentTimeMillis());
//		quicksort(hundred);
//		System.out.println("Sort Test End:  " + System.currentTimeMillis());
//		
//		for(int i = 0; i < thousand.length; i++){
//			thousand[i] = rand.nextInt(1000);
//		}
//		
//		System.out.println("");
//		System.out.println("int[1000]");
//		
//		System.out.println("Start Time:  " + System.currentTimeMillis());
//		selectionSort(thousand);
//		System.out.println("Selection Sort End:  " + System.currentTimeMillis());
//		
//		System.out.println("Start Time:  " + System.currentTimeMillis());
//		bubbleSort(thousand);
//		System.out.println("Bubble Sort End:  " + System.currentTimeMillis());
//		
//		System.out.println("Start Time:  " + System.currentTimeMillis());
//		insertionSort(thousand);
//		System.out.println("Insertion Sort End:  " + System.currentTimeMillis());
//		
//		System.out.println("Start Time:  " + System.currentTimeMillis());
//		mergeSort(thousand);
//		System.out.println("Merge Sort End:  " + System.currentTimeMillis());
//		
//		System.out.println("Start Time:  " + System.currentTimeMillis());
//		quicksort(thousand);
//		System.out.println("Sort Test End:  " + System.currentTimeMillis());
//		
//		for(int i = 0; i < ten4.length; i++){
//			ten4[i] = rand.nextInt(1000);
//		}
//		
//		System.out.println("");
//		System.out.println("int[10000]");
//		System.out.println("Start Time:  " + System.currentTimeMillis());
//		
//		System.out.println("Start Time:  " + System.currentTimeMillis());
//		selectionSort(ten4);
//		System.out.println("Selection Sort End:  " + System.currentTimeMillis());
//		
//		System.out.println("Start Time:  " + System.currentTimeMillis());
//		bubbleSort(ten4);
//		System.out.println("Bubble Sort End:  " + System.currentTimeMillis());
//		
//		System.out.println("Start Time:  " + System.currentTimeMillis());
//		insertionSort(ten4);
//		System.out.println("Insertion Sort End:  " + System.currentTimeMillis());
//		
//		System.out.println("Start Time:  " + System.currentTimeMillis());
//		mergeSort(ten4);
//		System.out.println("Merge Sort End:  " + System.currentTimeMillis());
//		
//		System.out.println("Start Time:  " + System.currentTimeMillis());
//		quicksort(ten4);
//		System.out.println("Sort Test End:  " + System.currentTimeMillis());
//		
//		for(int i = 0; i < ten5.length; i++){
//			ten5[i] = rand.nextInt(100);
//		}
//		
//		System.out.println("");
//		System.out.println("int[100000]");
//	
//		System.out.println("Start Time:  " + System.currentTimeMillis());
//		selectionSort(ten5);
//		System.out.println("Selection Sort End:  " + System.currentTimeMillis());
//		
//		System.out.println("Start Time:  " + System.currentTimeMillis());
//		bubbleSort(ten5);
//		System.out.println("Bubble Sort End:  " + System.currentTimeMillis());
//		
//		System.out.println("Start Time:  " + System.currentTimeMillis());
//		insertionSort(ten5);
//		System.out.println("Insertion Sort End:  " + System.currentTimeMillis());
//		
//		System.out.println("Start Time:  " + System.currentTimeMillis());
//		mergeSort(ten5);
//		System.out.println("Merge Sort End:  " + System.currentTimeMillis());
//		
//		System.out.println("Start Time:  " + System.currentTimeMillis());
//		quicksort(ten5);
//		System.out.println("Sort Test End:  " + System.currentTimeMillis());
//	}
}
