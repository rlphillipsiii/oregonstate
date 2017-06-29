package com.thirdgen.chip8;

public class Stack {
	private static final int STACK_DEPTH = 16;
	private static final int STACK_MEMORY_SIZE = 16;
	
	private Register m_memory[];
	private int m_pointer;
	
	public Stack() {
		m_pointer = 0;
		
		m_memory = new Register[STACK_DEPTH];
		for (int i = 0; i < m_memory.length; i++) {
			m_memory[i] = new Register(STACK_MEMORY_SIZE, 0, "Stack_" + Integer.toHexString(i));
		}
	}
	
	public void push(long value) throws ArrayIndexOutOfBoundsException {
		if (m_pointer == STACK_DEPTH) {
			throw new ArrayIndexOutOfBoundsException("Cannot push to stack.  Stack is full.");
		}
		m_memory[m_pointer++].set(value);
	}
	
	public long pop() throws ArrayIndexOutOfBoundsException {
		if (m_pointer == 0) {
			throw new ArrayIndexOutOfBoundsException("Cannot pop from stack.  Stack is empty.");
		}
		return m_memory[--m_pointer].getValue();
	}
}
