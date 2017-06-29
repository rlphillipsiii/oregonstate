import java.io.FileNotFoundException;

public class AnagramSolver {
	private Dictionary dict;
	private String algorithm;
	
	public AnagramSolver(Dictionary dictionary) throws FileNotFoundException {
		dict = dictionary;
	}

	public String solveHelper(String word, char string[]){
		if(word.length() == 0)
			if(dict.contains(String.valueOf(string)))
				return String.valueOf(string);
			else
				return null;
		
		for(int i = 0; i < word.length(); i++){
			string[string.length - word.length()] = word.charAt(i);
			String temp = word.substring(0, i) + word.substring(i + 1);
			
			if((temp = solveHelper(temp, string)) != null)
				 return temp;
		}
		
		return null;
	}
	
	public String solve(String s){
		return solveHelper(s, new char[s.length()]);
	}
	
	public double getAverageLookUpTime(){
		return dict.getAverageLookupTime();
	}
	
	public String getAlgorithm(){
		return algorithm;
	}
	
	public static void main(String[] args) throws FileNotFoundException {
		if(args.length < 3){
			System.out.println("Usage:  < anagram > < dictionary file > < dictionary type >");
			return;
		}
		
		AnagramSolver solver;
		String algorithm;
		
		switch(args[2].charAt(0)){
		case 'l':
			solver = new AnagramSolver(new LinearDictionary(args[1]));
			algorithm = "Linear Search";
			break;
		case 'b':
			solver = new AnagramSolver(new BinaryDictionary(args[1]));
			algorithm = "Binary Search";
			break;
		case 'h':
			solver = new AnagramSolver(new HashDictionary(args[1]));
			algorithm = "Hash Search";
			break;
		default:
			solver = new AnagramSolver(new LinearDictionary(args[1]));
			algorithm = "Linear Search";
			break;
		}
		
		String anagram;
		
		System.out.println("Searching using a " + algorithm);
		if((anagram = solver.solve(args[0])) != null){
			System.out.println("Original Anagram:  \"" + args[0] + "\"");
			System.out.println("Solved Anagram:  \"" + anagram + "\"");
		}else
			System.out.println("Word Not Found!");
		
		System.out.println("Average Time:  " + solver.getAverageLookUpTime());
	}
}
