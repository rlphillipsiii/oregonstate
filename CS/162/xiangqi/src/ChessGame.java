/**
 * @author 
 *
 * Interface defining required methods that a chess game must implement
 */
public interface ChessGame {
	
	/**
	 * Represenation of the two players
	 */
	public enum Side {
		NORTH,
		SOUTH
	};
		
	/**
	 * Determines if the piece is legal to play.
	 * 
	 * @return
	 * 		A boolean indicating whether it's legal to play the piece
	 */
	public boolean legalPieceToPlay(ChessPiece piece);
	
	/**
	 * Moves the given piece to the new coordinates if the move is legal.
	 * 
	 * @param piece
	 * 		The piece to be moved
	 * 
	 * @param row
	 * 		The new row
	 * 
	 * @param col
	 * 		The new column
	 * 
	 * @return
	 * 		A boolean indicating whether or not the piece was moved
	 */
	public boolean makeMove(ChessPiece piece, int row, int col);
}
