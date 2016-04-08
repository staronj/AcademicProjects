/**
 * Copyright Jakub Staroń, 2015
 */

public class TerminalCharacter extends GrammarCharacter {
	TerminalCharacter(char c) {
		if (isValidTerminalCharacter(c)) {
			this.character = c;
		} else {
			throw new IllegalArgumentException("Invalid terminal character: " + c);
		}
	}

	public static boolean isValidTerminalCharacter(char c) {
		return Character.isAlphabetic(c) && Character.isLowerCase(c);
	}

	public boolean isTerminal() {
		return true;
	}
}
