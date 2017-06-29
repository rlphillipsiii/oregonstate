import java.io.FileNotFoundException;

public abstract class Dictionary {
	protected int searches;
	protected double time;
	
	public abstract boolean contains(String s);
	public abstract Object readFile(String path) throws FileNotFoundException;
	
	public double getAverageLookupTime(){
		if(searches == 0)
			return 0;
		
		return time/1000/searches;
	}
}
