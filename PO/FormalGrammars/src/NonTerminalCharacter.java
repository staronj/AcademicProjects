/**
 * Copyright Jakub Staroń, 2015
 */

public class NonTerminalCharacter extends GrammarCharacter {
	NonTerminalCharacter(char c) {
		if (isValidNonTerminalCharacter(c)) {
			this.character = c;
		} else {
			throw new IllegalArgumentException("Invalid non-terminal character: " + c);
		}
	}

	public static boolean isValidNonTerminalCharacter(char c) {
		return Character.isAlphabetic(c) && Character.isUpperCase(c);
	}

	public boolean isTerminal() {
		return false;
	}
}
