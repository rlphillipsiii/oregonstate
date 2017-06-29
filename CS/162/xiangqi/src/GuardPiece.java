/**
 * @author 
 *
 * Implementation of the movement rules for the Guard piece
 */
public class GuardPiece extends BoxedPiece {
	public GuardPiece(ChessGame.Side owner, ChessBoard board) {
		super(owner, board, "G");
	}

	@Override
	public boolean isLegalNonCaptureMove(int row, int col) {
		/* only can move straight diagonally */
		if (!isMovingStraightDiagonally(row, col)) {
			return false;
		}
		
		/* have to stay in the bounds */
		if (!super.isLegalNonCaptureMove(row, col)) {
			return false;
		}
		
		/* only allowed to move one space at a time */
		if (Math.abs(m_row-row) != 1 || Math.abs(m_col-col) != 1) {
			return false;
		}
		
		return true;
	}

	@Override
	public boolean isLegalCaptureMove(int row, int col) {
		return isLegalNonCaptureMove(row, col);
	}
}
