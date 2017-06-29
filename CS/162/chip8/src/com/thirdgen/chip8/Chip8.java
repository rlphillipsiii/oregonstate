package com.thirdgen.chip8;

public class Chip8 {
	public static final int CHIP8_MEMORY_COUNT = 4096;
	public static final int CHIP8_MEMORY_BLOCK_SIZE = 8;
	public static final int CHIP8_VREGISTER_COUNT = 16;
	public static final int CHIP8_VREGISTER_BLOCK_SIZE = 8;
	public static final int CHIP8_IREG_SIZE = 16;
	public static final int CHIP8_PC_SIZE = 16;
	public static final int CHIP8_DELAY_REGISTER_SIZE = 8;
	public static final int CHIP8_SOUND_REGISTER_SIZE = 8;
	
	public static final int CHIP8_ENTRY_ADDRESS = 0x0200;
	
	public static final int CHIP8_FONTSET[] =
		{ 
		  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		  0x20, 0x60, 0x20, 0x20, 0x70, // 1
		  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
		};
	
	private Register m_memory[];
	private Register m_V[];
	
	private byte m_pixels[];
	private byte m_keys[];
		
	private Register m_Ireg;
	private Register m_pc;

	private Stack m_stack;
	
	private int m_opcode;
	private int m_subop;
	
	private Register m_delay;
	private Register m_sound;
	
	private boolean m_redraw;
	
	public Chip8() {
		m_pixels = new byte[2048];
		
		m_memory = new Register[CHIP8_MEMORY_COUNT];
		for (int i = 0; i < m_memory.length; i++) {
			m_memory[i] = new Register(CHIP8_MEMORY_BLOCK_SIZE, 0, "Memory_" + Integer.toHexString(i));
		}
		
		m_V = new Register[CHIP8_VREGISTER_COUNT];
		for (int i = 0; i < m_V.length; i++) {
			m_V[i] = new Register(CHIP8_VREGISTER_BLOCK_SIZE, 0, "V_" + Integer.toHexString(i));
		}
		
		for (int i = 0; i < CHIP8_FONTSET.length; i++) {
			m_memory[i].set((CHIP8_FONTSET[i] & 0xFF));
		}

		m_Ireg = new Register(CHIP8_IREG_SIZE, 0, "I");
		m_pc = new Register(CHIP8_PC_SIZE, CHIP8_ENTRY_ADDRESS, "PC");
		
		m_delay = new Register(CHIP8_DELAY_REGISTER_SIZE, 0, "Delay");
		m_sound = new Register(CHIP8_SOUND_REGISTER_SIZE, 0, "Sound");
		
		m_stack = new Stack();
		
		m_keys = new byte[16];

		Register.enableDebugMessages(false);
		OpCode.enableDebugMessages(false);
		
		m_redraw = true;
	}
	
	public void doEmulateCycle() throws Exception {
		fetch();
		execute();
	}
	
	public void updateKeys(byte keys[]) {
		for (int i = 0; i < this.m_keys.length; i++)
			this.m_keys[i] = keys[i];
	}
	
	public boolean doesScreenNeedRedraw() {
		return m_redraw;
	}
	
	public void clearRedrawFlag() {
		m_redraw = false;
	}
	
	public void setMemory(int index, long value) {
		m_memory[index].set(value);
	}
	
	public byte[] getPixelArray() {
		return m_pixels;
	}
	
	public void decrementDelay() {
		if (m_delay.getValue() == 0) {
			return;
		}
		m_delay.decrement(1);
	}
	
	public void decrementSound() {
		if (m_sound.getValue() == 0) {
			return;
		}
		//TODO: play a beeping sound
		m_sound.decrement(1);
	}
	
	private void fetch() {
		int address = (int)m_pc.getValue();
		
		long upper = m_memory[address].getValue();
		long lower = m_memory[address + 1].getValue();
		
		int value = (int) (((upper << 8) & 0xFFFF) | (lower & 0xFF));
		m_opcode = value & 0xF000;
		m_subop  = value & 0x0FFF;
		
		m_pc.increment(2);
	}
	
	private void execute() throws OpCodeException {	
		switch (m_opcode) {
		case 0x0000: execute0x0000(m_subop); break;
		case 0x1000: execute0x1000(m_subop); break;
		case 0x2000: execute0x2000(m_subop); break;
		case 0x3000: execute0x3000(m_subop); break;
		case 0x4000: execute0x4000(m_subop); break;
		case 0x5000: execute0x5000(m_subop); break;
		case 0x6000: execute0x6000(m_subop); break;
		case 0x7000: execute0x7000(m_subop); break;
		case 0x8000: execute0x8000(m_subop); break;
		case 0x9000: execute0x9000(m_subop); break;
		case 0xA000: execute0xA000(m_subop); break;
		case 0xB000: execute0xB000(m_subop); break;
		case 0xC000: execute0xC000(m_subop); break;
		case 0xD000: execute0xD000(m_subop); break;
		case 0xE000 :execute0xE000(m_subop); break;
		case 0xF000: execute0xF000(m_subop); break;
		default:
			throw new OpCodeException("Unknown opcode:  " + Integer.toHexString(m_opcode | m_subop));
		}
	}
	
	private void execute0x0000(int subcode) throws OpCodeException 
	{
		switch (subcode) {
		case 0xE0: 
			OpCode.clearPixels(m_pixels); 
			m_redraw = true;	
			break;
			
		case 0xEE: 
			OpCode.ret(m_pc, m_stack); 
			break;
			
		default:
			throw new OpCodeException("Unknown opcode:  " + Integer.toHexString(m_opcode | m_subop));
		}
	}
	private void execute0x1000(int subcode)
	{
		OpCode.jump(m_pc, subcode);
	}
	private void execute0x2000(int subcode)
	{
		OpCode.call(m_pc, m_stack, subcode);
	}
	private void execute0x3000(int subcode)
	{
		OpCode.skipIfEqual(m_pc, m_V, subcode);
	}
	private void execute0x4000(int subcode)
	{
		OpCode.skipIfNotEqual(m_pc, m_V, subcode);
	}
	private void execute0x5000(int subcode)
	{
		OpCode.skipIfRegEqual(m_pc, m_V, subcode);
	}
	private void execute0x6000(int subcode)
	{
		OpCode.setRegImmediate(m_V, subcode);
	}
	private void execute0x7000(int subcode)
	{
		OpCode.addRegImmediate(m_V, subcode);
	}
	private void execute0x8000(int subcode) throws OpCodeException 
	{
		switch (subcode & 0x000F) {
		case 0x0: OpCode.setReg(m_V, subcode);      break;
		case 0x1: OpCode.orReg(m_V, subcode);       break;
		case 0x2: OpCode.andReg(m_V, subcode);      break;
		case 0x3: OpCode.xorReg(m_V, subcode);      break;
		case 0x4: OpCode.addReg(m_V, subcode);      break;			
		case 0x5: OpCode.subReg(m_V, subcode);      break;
		case 0x6: OpCode.rotateRight(m_V, subcode); break;
		case 0x7: OpCode.addNegative(m_V, subcode); break;
		case 0xE: OpCode.rotateLeft(m_V, subcode);  break;
		default:
			throw new OpCodeException("Unknown opcode:  " + Integer.toHexString(m_opcode | m_subop));
		}
	}
	private void execute0x9000(int subcode)
	{
		OpCode.skipIfRegNotEqual(m_pc, m_V, subcode);
	}
	private void execute0xA000(int subcode)
	{
		OpCode.setI(m_Ireg, subcode);
	}
	private void execute0xB000(int subcode)
	{
		OpCode.jumpRelative(m_pc, m_V, subcode);
	}
	private void execute0xC000(int subcode)
	{
		OpCode.setRegRandom(m_V, subcode);
	}
	private void execute0xD000(int subcode) 
	{
		OpCode.updatePixels(m_memory, m_pixels, m_V, m_Ireg, subcode);
		m_redraw = true;
	}
	private void execute0xE000(int subcode) throws OpCodeException 
	{
		switch (subcode & 0x00FF) {
		case 0x9E: OpCode.skipIfButtonPressed(m_keys, m_V, m_pc, subcode);    break;
		case 0xA1: OpCode.skipIfButtonNotPressed(m_keys, m_V, m_pc, subcode); break;
		default:
			throw new OpCodeException("Unknown opcode:  " + Integer.toHexString(m_opcode | m_subop));
		}
	}
	private void execute0xF000(int subcode) throws OpCodeException 
	{
		switch (subcode & 0x00FF) {
		case 0x07: OpCode.setRegToDelayTimer(m_V, m_delay, subcode);      break;
		case 0x0A: OpCode.waitForKeyPress(m_V, m_keys, m_pc, subcode);    break;
		case 0x15: OpCode.setDelayToReg(m_V, m_delay, subcode);           break;
		case 0x18: OpCode.setSoundToReg(m_V, m_sound, subcode);           break;
		case 0x1E: OpCode.addRegToI(m_V, m_Ireg, subcode);                break;
		case 0x29: OpCode.setIToSprite(m_V, m_Ireg, subcode);             break;
		case 0x33: OpCode.storeBCD(m_memory, m_V, m_Ireg, subcode);       break;
		case 0x55: OpCode.storeRegisters(m_memory, m_V, m_Ireg, subcode); break;
		case 0x65: OpCode.fillRegisters(m_memory, m_V, m_Ireg, subcode);  break;
		default:
			throw new OpCodeException("Unknown opcode:  " + Integer.toHexString(m_opcode | m_subop));
		}
	}
}
