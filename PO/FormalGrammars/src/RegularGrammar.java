/**
 * Copyright Jakub Staroń, 2015
 */

public class RegularGrammar extends ContextFreeGrammar {
	public RegularGrammar(ContextFreeGrammar grammar) {
		super(grammar);
		if (!grammar.ifRegular()) {
			throw new IllegalArgumentException("RegularGrammar: grammar is not regular!");
		}
	}
}