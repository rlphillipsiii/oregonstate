/**
 * @author 
 *
 * Implementation of the movement rules for the Elephant piece
 */
public class ElephantPiece extends ChessPiece {
	public ElephantPiece(ChessGame.Side owner, ChessBoard board) {
		super(owner, board, "E");
	}

	@Override
	public boolean isLegalNonCaptureMove(int row, int col) {
		return _isLegalMove(row, col, 0);
	}

	@Override
	public boolean isLegalCaptureMove(int row, int col) {
		return _isLegalMove(row, col, 1);
	}
	
	/**
	 * Checks to see if the destination is a legal move.
	 * 
	 * @param row
	 * 		The destination row
	 * 
	 * @param col
	 * 		The destination column
	 * 
	 * @param pieces
	 * 		The number of pieces allowed to be in the path to the destination
	 * 
	 * @return
	 * 		A boolean indicating whether the move is legal or not
	 */
	private boolean _isLegalMove(int row, int col, int pieces) {
		/* has to move straigth diagonally */
		if (!isMovingStraightDiagonally(row, col)) {
			return false;
		}
		
		/* can't go past its half of the board */
		if ((row > 4 && m_owner == ChessGame.Side.NORTH) ||
				(row < 5 && m_owner == ChessGame.Side.SOUTH)) {
			return false;
		}
		
		/* moves two pieces at once */
		if (Math.abs(m_row-row) != 2 || Math.abs(m_col-col) != 2) {
			return false;
		}
		
		/* check for pieces in the path */
		if (piecesInPath(row, col) != pieces) {
			return false;
		}
		
		return true;
	}
}
