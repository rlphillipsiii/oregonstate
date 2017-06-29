/**
 * @author 
 *
 * Implementation of the movement rules for the Pawn piece
 */
public class PawnPiece extends ChessPiece {

	public PawnPiece(ChessGame.Side owner, ChessBoard board) {
		super(owner, board, "P");
	}

	@Override
	public boolean isLegalNonCaptureMove(int row, int col) {
		/* can't move diagonally */
		if (isMovingDiagonally(row, col)) {
			return false;
		}
		
		/* can only go away from home */
		if ((row == m_row+1 && m_owner == ChessGame.Side.NORTH) ||
				(row == m_row-1 && m_owner == ChessGame.Side.SOUTH)) {
			return true;
		}
		
		/* allow horizontal moves after passing the halfway point */
		if ((m_row > 4 && m_owner == ChessGame.Side.NORTH) ||
				(m_row < 5 && m_owner == ChessGame.Side.SOUTH)) {
			if (Math.abs(m_col-col) == 1) {
				return true;
			}
		}
		
		return false;
	}

	@Override
	public boolean isLegalCaptureMove(int row, int col) {
		return isLegalNonCaptureMove(row, col);
	}

}
