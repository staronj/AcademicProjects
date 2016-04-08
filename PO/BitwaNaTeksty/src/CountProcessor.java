/**
 * Copyright Jakub Staroń, 2015
 */

package bitwaNaTeksty;

import java.util.*;
import java.util.stream.Stream;


/**
 * @author Kuba
 * Procesor tektów zliczaj¹cy ró¿ne wyrazy.
 */
public class CountProcessor implements Processor {
	/* (non-Javadoc)
	 * @see bitwaNaTeksty.Processor#process(java.util.Collection)
	 */
	public void process(Collection<TextSource> sources)
	{
		System.out.println("count:");
		for(TextSource source : sources)		
			process(source);		
	}
	
	/* (non-Javadoc)
	 * @see bitwaNaTeksty.Processor#process(bitwaNaTeksty.TextSource)
	 */
	public void process(TextSource source) {
		Set<String> vocabulary = new TreeSet<String>();
		Stream<String> wordStream = source.wordsStream();
				
		wordStream.forEach(str -> vocabulary.add(str));
		
		System.out.println(source.getArtistName() + " " + Integer.toString(vocabulary.size()));
	}
}
