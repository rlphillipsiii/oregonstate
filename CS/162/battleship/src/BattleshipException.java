
/**
 * This class represents an exception class specifically for the Battleship game.
 * Used in CS 162 Assn2
 */


public class BattleshipException extends Exception {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	/**
	 * Constructor for BattleshipException
 	 * @param message Error message for the Exception
	 */
	public BattleshipException(String message) {
		super(message);
	}
}
