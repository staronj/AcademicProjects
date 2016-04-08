/**
 * Copyright Jakub Staroń, 2015
 */

package bitwaNaTeksty;
import java.util.*;
import java.util.stream.Stream;


/**
 * @author Kuba
 * Procesor tekstów "top5".
 */
public class Top5Processor implements Processor {
	/* (non-Javadoc)
	 * @see bitwaNaTeksty.Processor#process(java.util.Collection)
	 */
	public void process(Collection<TextSource> sources)
	{
		System.out.println("top5:");
		for(TextSource source : sources)		
			process(source);		
	}
	
	/* (non-Javadoc)
	 * @see bitwaNaTeksty.Processor#process(bitwaNaTeksty.TextSource)
	 */
	public void process(TextSource source) {
		Map<String, Integer> counting = new TreeMap<String, Integer>();
		Stream<String> wordStream = source.wordsStream();
				
		wordStream.forEach(str -> {
			if(counting.containsKey(str)) {
				Integer oldValue = counting.get(str);
				counting.put(str, oldValue + 1);
			}
			else {
				counting.put(str, 1);
			}
		});
		
		ArrayList<Map.Entry<String, Integer>> list = new ArrayList<Map.Entry<String, Integer>>(counting.entrySet());
		list.sort( (a, b) -> {
			int t = b.getValue().compareTo(a.getValue());
			return (t == 0)? a.getKey().compareTo(b.getKey()) : t;
		});
		
		List<Map.Entry<String, Integer>> sublist = list.subList(0, Integer.min(5, list.size()));
		System.out.println(source.getArtistName());
		System.out.println(sublist.toString());
	}
}
