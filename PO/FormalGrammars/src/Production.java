/**
 * Copyright Jakub Staroń, 2015
 */

import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.lang.StringBuilder;

public class Production {
	public Production(NonTerminalCharacter origin, List<GrammarCharacter> product) {
		this.origin = origin;
		this.product = new ArrayList<GrammarCharacter>(product);
	}

	/*
	 * Funkcje pozwalaj¹ce badaæ, czy dana produkcja jest poprawn¹ produkcj¹ w
	 * jakiejœ postaci gramatyki
	 */
	public boolean isLeftRegularProduction() {
		if (isEmptyProduction()) {
			return true;
		} else if (product.size() == 1) {
			return product.get(0).isTerminal();
		} else if (product.size() == 2) {
			return !product.get(0).isTerminal() && product.get(1).isTerminal();
		} else {
			return false;
		}
	}

	public boolean isRightRegularProduction() {
		if (isEmptyProduction()) {
			return true;
		} else if (product.size() == 1) {
			return product.get(0).isTerminal();
		} else if (product.size() == 2) {
			return product.get(0).isTerminal() && !product.get(1).isTerminal();
		} else {
			return false;
		}
	}

	public boolean isChomskyProduction() {
		if (product.size() == 1) {
			return product.get(0).isTerminal();
		} else if (product.size() == 2) {
			return !product.get(0).isTerminal() && !product.get(1).isTerminal();
		} else {
			return false;
		}
	}

	public boolean isGreibachProduction() {
		if (isEmptyProduction())
			return false;

		GrammarCharacter first = product.get(0);
		return first.isTerminal();
	}

	public boolean isEmptyProduction() {
		return product.isEmpty();
	}

	public NonTerminalCharacter getOrigin() {
		return origin;
	}

	public List<GrammarCharacter> getProduct() {
		return product;
	}
	
	//Zwraca produkcjê z odciêciem pierwszego znaku produkcji
	public List<GrammarCharacter> getProductSuffix() {
		if(isEmptyProduction())
		{
			throw new IllegalStateException("Production.getProductionSuffix - empty production!");
		}
		else
		{
			return product.subList(1, product.size());
		}
	}

	public String toString() {
		StringBuilder builder = new StringBuilder();
		builder.append(origin.toString());
		builder.append("->");
		if (isEmptyProduction()) {
			builder.append('&');
		} else {
			for (GrammarCharacter ch : product) {
				builder.append(ch.toString());
			}
		}
		return builder.toString();
	}

	public boolean isProductiveProduction(Set<GrammarCharacter> productiveNonTerminals) {
		return product.stream().allMatch(
				ch -> (ch.isTerminal() || productiveNonTerminals.contains(ch)));
	}
	
	/*
	 * Zwraca pierwszy znak produkcji.
	 */
	public GrammarCharacter firstCharacter()
	{
		if(isEmptyProduction())
		{
			throw new IllegalStateException("Production.firstCharacter - empty production!");
		}
		else
		{
			return product.get(0);
		}
	}

	/*
	 * Generuje wszystkie produkcje, które mo¿na otrzymaæ podstawiaj¹c
	 * pod pierwszy symbol produkcji produkcje z listy
	 */
	public List<Production> substitute(List<Production> list)
	{
		if(isEmptyProduction())
			throw new IllegalStateException("Production.substitute - empty production!");
		
		NonTerminalCharacter firstCharacter = (NonTerminalCharacter)firstCharacter();
		List<Production> result = new ArrayList<Production>();
		for(Production production : list)
		{
			if(firstCharacter.equals(production.getOrigin()))
			{
				List<GrammarCharacter> newProduct = new ArrayList<GrammarCharacter>(production.getProduct());
				newProduct.addAll( getProductSuffix() );
				
				result.add(new Production(getOrigin(), newProduct));
			}
		}
		
		return result;		
	}
	
	 
	private NonTerminalCharacter origin;
	private ArrayList<GrammarCharacter> product;
}
