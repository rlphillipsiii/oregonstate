/**
 * @author
 * 
 * Interface for defining functions that must be implemented by a ChessBoardDisplay
 */
import javax.swing.JButton;

public interface ChessBoardDisplay {
	/**
	 * Formats the JButton with the style corresponding to an empty square on the
	 * chess board.
	 * 
	 * @param button
	 * 		The JButton to be formatted
	 * 
	 * @param row
	 * 		The row in the grid of the JButton to be formatted
	 * 
	 * @ param col
	 * 		The column in the grid of the JButton to be formatted
	 */
	void displayEmptySquare(JButton button, int row, int col);
	
	/**
	 * Formats the JButton with the style corresponding to the given chess piece.
	 * 
	 * @param button
	 * 		The JButton to be formatted
	 * 
	 * @param piece
	 * 		The chess piece that has the formatting details for the button
	 * 
	 * @param row
	 * 		The row in the grid of the JButton to be formatted
	 * 
	 * @ param col
	 * 		The column in the grid of the JButton to be formatted
	 */
	void displayFilledSquare(JButton button, ChessPiece piece, int row, int col);
	
	/**
	 * Highlights or removes highlighting of the JButton based on the given state and
	 * format of the JButton.
	 * 
	 * @param state
	 * 		Boolean indicating whether highlighting is being done or taken away
	 * 
	 * @param button
	 * 		The button to be formatted
	 * 
	 * @param piece
	 * 		The chess piece that has the formatting details for the button
	 * 
	 * @param row
	 * 		The row in the grid of the JButton to be formatted
	 * 
	 * @ param col
	 * 		The column in the grid of the JButton to be formatted
	 */
	void highlightSquare(boolean state, JButton button, ChessPiece piece, int row, int col);
}
