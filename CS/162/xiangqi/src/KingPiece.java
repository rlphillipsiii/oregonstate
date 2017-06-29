/**
 * @author 
 *
 * Implementation of the movement rules for the King piece
 */
public class KingPiece extends BoxedPiece {

	public KingPiece(ChessGame.Side owner, ChessBoard board) {
		super(owner, board, "K");
	}

	@Override
	public boolean isLegalNonCaptureMove(int row, int col) {
		/* can't move diagonally */
		if (isMovingDiagonally(row, col)) {
			return false;
		}
		
		/* have to stay in the bounds */
		if (!super.isLegalNonCaptureMove(row, col)) {
			return false;
		}
		
		/* can only move on square at once */
		if (Math.abs(m_row-row) == 1 || Math.abs(m_col-col) == 1) {
			return true;
		}
		
		return false;
	}

	@Override
	public boolean isLegalCaptureMove(int row, int col) {
		return isLegalNonCaptureMove(row, col);
	}
}
