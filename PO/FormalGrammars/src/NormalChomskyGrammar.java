/**
 * Copyright Jakub Staroń, 2015
 */

import java.util.*;
import java.util.function.Predicate;


/*
 * Pomocnicza klasa, przekszta³caj¹ca gramatykê w postaci Chomskyego
 * na gramatykê w postaci Greibach
 */
class ToGreibachConversion extends ContextFreeGrammar
{
	public ToGreibachConversion(NormalChomskyGrammar grammar)
	{
		super(grammar);
		nonTerminalsOrder = new LinkedList<GrammarCharacter>(nonTerminalSymbols);
		Collections.sort(nonTerminalsOrder);
		String alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		freeCharacters = new TreeSet<Character>();
		for(Character ch : alphabet.toCharArray())		
			freeCharacters.add(ch);
		
		for(GrammarCharacter ch : nonTerminalSymbols)
			freeCharacters.remove(ch.getChar());
	}
	
	public char pollFreeChar()
	{//za³o¿enia treœci gwarantuj¹, ¿e wywo³anie tej funkcji zawsze zakoñczy siê pomyœlnie
		Iterator<Character> it = freeCharacters.iterator();
		char c = it.next();
		it.remove();
		return c;		
	}
	
	public void firstPhase()
	{
		List<GrammarCharacter> nonTerminalsOrderCopy = new ArrayList<GrammarCharacter>(nonTerminalsOrder);
		for(GrammarCharacter ch : nonTerminalsOrderCopy)
		{
			firstPhaseFirstStep(ch);
			firstPhaseSecondStep(ch);
		}
	}
	
	public void firstPhaseFirstStep(GrammarCharacter ch)
	{//przepychamy wszystkie produkcje do stanu x_i -> x_j... gdzie j >= i
		Predicate<Production> validProductionPredicate = p -> (p.firstCharacter().isTerminal() || ch.compareTo(p.firstCharacter()) <= 0);
		List<Production> validProductions = new ArrayList<Production>();
		Queue<Production> invalidProductions = new LinkedList<Production>();
		invalidProductions.addAll(productions.get(ch));
		while(!invalidProductions.isEmpty())
		{
			Production production = invalidProductions.poll();
			if(validProductionPredicate.test(production))
			{
				validProductions.add(production);
			}
			else
			{
				List<Production> productionsForSubstitute = productions.get(production.firstCharacter());
				invalidProductions.addAll( production.substitute(productionsForSubstitute) );
			}
		}
		productions.put(ch, validProductions);
	}
	
	public void firstPhaseSecondStep(GrammarCharacter ch)
	{//przepychamy wszystkie produkcje do stanu x_i -> x_j... gdzie j > i
		Predicate<Production> validProductionPredicate = p -> (!p.firstCharacter().equals(ch));
		List<Production> validProductions = new ArrayList<Production>();
		List<Production> invalidProductions = new ArrayList<Production>();
		for(Production production : productions.get(ch))
		{
			if(validProductionPredicate.test(production))
			{
				validProductions.add(production);
			}
			else
			{
				invalidProductions.add(production);
			}
		}
		if(invalidProductions.isEmpty())
			return;
		
		//skoro gramatyka jest produktywna, to w tym miejscu validProductions.size() > 0
		assert(validProductions.size() > 0);
		
		GrammarCharacter P = new NonTerminalCharacter(pollFreeChar());		
		List<Production> finalXProductions = new ArrayList<Production>(validProductions);
		List<Production> finalPProductions = new ArrayList<Production>(validProductions);
		
		for(Production valid : validProductions)
		{//dodajemy produkcje X -> <poprawny> P
			List<GrammarCharacter> newProduct = new ArrayList<GrammarCharacter>(valid.getProduct());
			newProduct.add(P);
			finalXProductions.add( new Production((NonTerminalCharacter)ch, newProduct) );
		}
		
		for(Production invalid : invalidProductions)
		{//dodajemy produkcje P->... i P->...P
			List<GrammarCharacter> newProduct = new ArrayList<GrammarCharacter>(invalid.getProductSuffix());
			finalPProductions.add( new Production((NonTerminalCharacter)P, newProduct) );
			newProduct.add(P);
			finalPProductions.add( new Production((NonTerminalCharacter)P, newProduct) );
		}
		
		nonTerminalsOrder.addFirst(P);
		
		productions.put(ch, finalXProductions);
		productions.put(P, finalPProductions);
	}
	
	public void secondPhase()
	{
		Predicate<Production> validProductionPredicate = p -> (p.firstCharacter().isTerminal());
		Collections.reverse(nonTerminalsOrder);
		for(GrammarCharacter ch : nonTerminalsOrder)
		{
			List<Production> actualProductions = productions.get(ch);
			List<Production> newProductions = new ArrayList<Production>();
			for(Production production : actualProductions)
			{
				if(validProductionPredicate.test(production))
				{
					newProductions.add(production);
				}
				else
				{
					List<Production> toSubstitute = productions.get(production.firstCharacter());
					newProductions.addAll( production.substitute(toSubstitute) ); 
				}
			}
			productions.put(ch, newProductions);
		}
	}

	public NormalGreibachGrammar result()
	{
		return new NormalGreibachGrammar(this);
	}
	
	private LinkedList<GrammarCharacter> nonTerminalsOrder;
	private Set<Character> freeCharacters;
}



public class NormalChomskyGrammar extends ContextFreeGrammar {
	public NormalChomskyGrammar(ContextFreeGrammar grammar) {
		super(grammar);
		if (!grammar.ifChomsky()) {			
			throw new IllegalArgumentException("RegularGrammar: grammar is not in Chomsky form!");
		}
	}
	
	public NormalGreibachGrammar toGreibach()
	{
		ToGreibachConversion conversion = new ToGreibachConversion(this);
		conversion.firstPhase();
		conversion.secondPhase();			
		return conversion.result();	
	}
}
