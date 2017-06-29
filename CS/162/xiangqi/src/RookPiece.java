/**
 * @author 
 *
 * Implementation of the movement rules for the Rook piece
 */
public class RookPiece extends ChessPiece {
	
	public RookPiece(ChessGame.Side owner, ChessBoard board) {
		super(owner, board, "R");
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
		if (isMovingDiagonally(row, col)) {
			return false;
		}
		
		if (piecesInPath(row, col) != pieces) {
			return false;
		}
		
		return true;
	}
}
