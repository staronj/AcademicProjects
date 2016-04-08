/**
 * Copyright Jakub Staroń, 2015
 */

package bitwaNaTeksty;

import java.util.stream.Stream;

/**
 * @author Kuba
 * ród³o tekstów z na³o¿onym filtrem.
 */
public class FilteredTextSource extends TextSource {
	/**
	 * @param source ród³o do filtrowania.
	 * @param filter Filtr,
	 */
	public FilteredTextSource(TextSource source, Filter filter) {
		this.source = source;
		this.filter = filter;
	}
	
	/* (non-Javadoc)
	 * @see bitwaNaTeksty.TextSource#wordsStream()
	 */
	public Stream<String> wordsStream() {
		return source.wordsStream().filter(str -> filter.isRelevant(str));
	}
	
	/* (non-Javadoc)
	 * @see bitwaNaTeksty.TextSource#getArtistName()
	 */
	public String getArtistName() {
		return source.getArtistName();
	}
	
	/* (non-Javadoc)
	 * @see bitwaNaTeksty.TextSource#songsStream()
	 */
	public Stream<Song> songsStream() {
		return source.songsStream();
	}
	
	private TextSource source;
	private Filter filter;
}
