/**
 * @author 
 *
 * Implementation of the movement rules for the Knight piece
 */
public class KnightPiece extends ChessPiece {
	public KnightPiece(ChessGame.Side owner, ChessBoard board) {
		super(owner, board, "N");
	}

	@Override
	public boolean isLegalNonCaptureMove(int row, int col) {
		/* first move is vertical? */
		if (Math.abs(m_row-row) == 2 && Math.abs(m_col-col) == 1) {
			int dir = (row > m_row) ? 1 : -1;
			
			/* check if our first square is clear */
			if (m_board.hasPiece(m_row+dir, m_col)) {
				return false;
			}
			
			return true;
		}
		
		/* first move is horizontal */
		if (Math.abs(m_row-row) == 1 && Math.abs(m_col-col) == 2) {
			int dir = (col > m_col) ? 1 : -1;
			
			/* check if our first square is clear */
			if (m_board.hasPiece(m_row, m_col+dir)) {
				return false;
			}
			
			return true;
		}
		
		return false;
	}

	@Override
	public boolean isLegalCaptureMove(int row, int col) {
		return isLegalNonCaptureMove(row, col);
	}
}
