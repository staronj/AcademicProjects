/**
 * Copyright Jakub Staroń, 2015
 */

package bitwaNaTeksty;
import java.util.stream.Stream;
import java.util.*;
import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;

/**
 * @author Kuba
 * ród³o tekstów z plików.
 */
public class FileTextSource extends TextSource {
	/**
	 * Konstruktor.
	 * @param path Œcie¿ka do folderu z artystami.
	 * @param artist Nazwa artysty.
	 */
	public FileTextSource(String path, String artist) {
		this.songList = new ArrayList<Song>();
		this.path = path;
		this.artist = artist;
		this.loadSongs();
	}
	
	/* (non-Javadoc)
	 * @see bitwaNaTeksty.TextSource#getArtistName()
	 */
	public String getArtistName()
	{
		return artist;
	}
	
	/* (non-Javadoc)
	 * @see bitwaNaTeksty.TextSource#songsStream()
	 */
	public Stream<Song> songsStream() {
		return songList.stream();
	}
		
	/**
	 * Wczytuje wszystkie piosenki danego arytsty.
	 */
	private void loadSongs() {
		File artistFolder = new File(path, artist);
		if(artistFolder.listFiles() == null) {
			System.err.println("Nie mogê za³adowaæ listy piosenek artysty " + artist);
			return;
		}
		for(File song : artistFolder.listFiles())
		{
			if(!getExtension(song.getPath()).equals("txt"))
				continue;
			
			String title = cutOffExtension(song.getName());
			
			try	{
				byte[] bytes = Files.readAllBytes(song.toPath());
				String text = new String(bytes, StandardCharsets.UTF_8);				
				songList.add(new Song(title, text));
			}
			catch(IOException e) {
				System.err.println("Error while loading file " + song.getPath());
			}		
		}
	}
	
	/**
	 * Pomocnicza funkcja ucinaj¹ca rozszerzenie pliku.
	 * @param filename Nazwa pliku.
	 * @return Nazwa pliku z odciêtym rozszerzeniem.
	 */
	private static String cutOffExtension(String filename) {
		int pos = filename.lastIndexOf(".");
		if (pos >= 0) {
			filename = filename.substring(0, pos);
		}
		return filename;
	}
	
	/**
	 * Pomocnicza funkcja zwracaj¹ca rozszerzenie pliku.
	 * @param filename Nazwa pliku.
	 * @return Rozszezrenie.
	 */
	private static String getExtension(String filename) {
		int pos = filename.lastIndexOf(".");
		if(pos >= 0)
			return filename.substring(pos + 1);
		else
			return "";
	}
	
	private String path;
	private String artist;
	private List<Song> songList;
}
