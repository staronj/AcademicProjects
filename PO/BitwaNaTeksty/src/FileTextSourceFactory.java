/**
 * Copyright Jakub Staroń, 2015
 */

package bitwaNaTeksty;

/**
 * @author Kuba
 * Fabryka Ÿróde³ tekstów z plików.
 */
public class FileTextSourceFactory implements TextSourceFactory {	
	/**
	 * Konstruktor.
	 * @param path Œcie¿ka do folderu z arytstami.
	 */
	public FileTextSourceFactory() {
		this.path = null;
	}
	
	/* (non-Javadoc)
	 * @see bitwaNaTeksty.TextSourceFactory#construct(java.lang.String)
	 */
	public TextSource construct(String artist) {
		return new FileTextSource(path, artist);
	}

	/* (non-Javadoc)
	 * @see bitwaNaTeksty.TextSourceFactory#init(java.lang.String)
	 */
	public void init(String path) {
		this.path = path;		
	}

	/* (non-Javadoc)
	 * @see bitwaNaTeksty.TextSourceFactory#ready()
	 */
	public boolean ready() {
		return this.path != null;
	}
	
	private String path;
}
