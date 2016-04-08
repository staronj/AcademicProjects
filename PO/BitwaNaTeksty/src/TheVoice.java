/**
 * Copyright Jakub Staroń, 2015
 */

package bitwaNaTeksty;
import java.io.File;
import java.io.IOException;
import java.util.*;

public class TheVoice {
	public static void main(String[] args) {		
	
		ArrayList<Processor> processors = new ArrayList<Processor>();
		TextSourceFactory sourceFactory = null;
		ArrayList<String> artists = new ArrayList<String>();
		ArrayList<Filter> filters = new ArrayList<Filter>();
		
		String sourceTypeString = "--source-type=";
		String sourceString = "--source=";
		String filtersString = "--filters=";
		String processorsString = "--processors=";

		
		for(String arg : args)
		{
			//System.out.print(arg + ";");
			if(arg.startsWith(sourceTypeString))
			{
				arg = arg.substring(sourceTypeString.length());
				switch(arg)
				{
				case "file":
					sourceFactory = new FileTextSourceFactory();
					break;
				default:
					System.out.println("Niepoprawna linia poleceñ.");
				}
			}
			else if(arg.startsWith(sourceString))
			{
				arg = arg.substring(sourceString.length());
				if(sourceFactory == null)
				{
					System.out.println("Argument source-type powinien poprzedziæ argument source.");
					return;
				}
				sourceFactory.init(arg);
			}
			else if(arg.startsWith(filtersString))
			{
				arg = arg.substring(filtersString.length());
				String[] files = arg.split(File.pathSeparator.toString());
				for(String file : files)
					filters.add(new FromFileFilter(file));
			}
			else if(arg.startsWith(processorsString))
			{
				arg = arg.substring(processorsString.length());
				String[] processorTypes = arg.split(",");
				for(String type : processorTypes)
				{
					switch(type)
					{
					case "top5":
						processors.add(new Top5Processor());
						break;
					case "count":
						processors.add(new CountProcessor());
						break;
					}
				}
			}
			else
			{
				artists.add(arg);			
			}
		}
		
		if(sourceFactory == null || !sourceFactory.ready())
		{
			System.out.println("Nie okreœlono Ÿród³a tektów.");
			return;
		}
		
		List<TextSource> sources = new ArrayList<TextSource>();
		
		for(String artist : artists)
			sources.add(sourceFactory.construct(artist));
		
		for(Filter filter : filters)		
			sources.replaceAll(source -> new FilteredTextSource(source, filter));
		
		for(Processor processor : processors)
		{
			processor.process(sources);
		}
	}
}
