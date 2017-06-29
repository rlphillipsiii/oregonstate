import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Hashtable;
import java.util.Scanner;


public class HashDictionary extends Dictionary {
	private Hashtable<Long, ArrayList<String>> dictionary;
	
	public HashDictionary(String path) throws FileNotFoundException{
		dictionary = readFile(path);
		
		super.time = 0;
		super.searches = 0;
	}
	
	public long hash(String s){
		long hash = 0;
		String hashable = s.toLowerCase();
		
		for(int i = 0; i < hashable.length(); i++)
			hash += (i + 1) * ((int) hashable.charAt(i));
		
		return hash;
	}
	
	@Override
	public boolean contains(String s) {
		super.searches ++;
		long start = System.currentTimeMillis();
		
		ArrayList<String> map;
		
		if((map = dictionary.get(hash(s))) == null){
			super.time += (System.currentTimeMillis() - start);
			
			return false;
		}
		
		for(int i = 0; i < map.size(); i++){
			if(map.get(i).equalsIgnoreCase(s)){
				super.time += (System.currentTimeMillis() - start);
				
				return true;
			}
		}
		
		super.time += (System.currentTimeMillis() - start);
		return false;
	}

	@Override
	public Hashtable<Long, ArrayList<String>> readFile(String path) throws FileNotFoundException {
		Hashtable<Long, ArrayList<String>> hashmap = new Hashtable<Long, ArrayList<String>>();
		
		Scanner in = new Scanner(new File(path));
		while(in.hasNextLine()){
			ArrayList<String> map;
			String line = in.nextLine();
			
			if((map = hashmap.get(hash(line))) == null){
				ArrayList<String> newMap = new ArrayList<String>();
				newMap.add(line);
				
				hashmap.put(hash(line), newMap);
			}else
				map.add(line);
		}
		
		in.close();
		return hashmap;
	}
}
