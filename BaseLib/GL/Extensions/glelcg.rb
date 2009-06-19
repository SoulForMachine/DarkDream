

def ParseCmdLine
	if ARGV.length == 0
		puts "\tusage: ruby glelcg.rb ext_header [filter_file]"
		return false
	end
	
	$inputFileName = ARGV[0]
	$filterFileName = ARGV[1]
	
	return true
end

def ParseFilterFile
	return if $filterFileName.nil? or $filterFileName.empty?

	begin
		in_file = File.new($filterFileName, "r");
		$extensionsToProcess = Array.new
		
		while line = in_file.gets
			line.strip!
			if not line.empty? and line[0] != ?#
				$extensionsToProcess.push line
			end
		end
		
		$extensionsToProcess.uniq!
	rescue
		puts "Error processing filter file. Processing all extensions."
	ensure
		in_file.close unless in_file.nil?
	end
end

def OkToProcess(ext)
	return true unless $extensionsToProcess
	result = $extensionsToProcess.find { |str| str == ext }
	if result
		$extensionsToProcess.delete result
		return true
	else
		return false
	end
end

def GenerateSource
	return if $inputFileName.nil? or $inputFileName.empty?
	
	begin
		in_file = File.new($inputFileName, "r")
		
		dot_index = $inputFileName.rindex('.')
		if dot_index == nil
			out_fpath = $inputFileName
		else
			out_fpath = $inputFileName.slice(0, dot_index)
		end
			
		bslash_index = out_fpath.rindex('\\')
		if bslash_index == nil
			out_fname = out_fpath
		else
			out_fname = out_fpath.slice(bslash_index + 1, out_fpath.length)
		end
		
		c_file = File.new("#{out_fname}_ptrs.inc", "w")
		h_file = File.new("#{out_fname}_ptrs.h", "w")
		loadall_file = File.new("#{out_fname}_load.inc", "w")
		
		loadall_file << "\nvoid #{out_fname}LoadAll()\n{\n"

		while line = in_file.gets()
			if line =~ /\s*#ifndef\s+(GL_|WGL_|GLX_)/
				ext_name = line.slice(/\s+(GL_|WGL_|GLX_)[a-z|A-Z|0-9|_]+/)
				ext_name.strip!
				
				line = in_file.gets()
				line = in_file.gets()
				
				if line =~ /#ifdef.*_PROTOTYPES/
					if OkToProcess(ext_name)
						h_file << "\n// #{ext_name}\n"
						h_file << "\nEXTPTR bool glextLoad_#{ext_name}();\n\n"
						
						c_file << "\n// #{ext_name}\n"
						c_file << "\nbool glextLoad_#{ext_name}()\n{\n"
						
						loadall_file << "\tglextLoad_#{ext_name}();\n"
						
						while line = in_file.gets()
							break if line =~ /\s*#endif.*/
							
							line = line.slice(/[a-z|A-Z|0-9|_]+\s*\(/)
							func_name = line.slice(/[a-z|A-Z|0-9|_]+/)
							func_type = "PFN#{func_name.upcase}PROC"
							
							h_file << "EXTPTR #{func_type} #{func_name};\n"
							
							c_file << "\tINIT_FUNC_PTR(#{func_name});\n"
						end
						
						c_file << "\n\treturn true;\n}\n"
					end
				end
			end
		end
		
		loadall_file << "}\n"
		
	rescue
		puts "Error occured while processing extension file."
	ensure
		in_file.close() unless in_file.nil?
		c_file.close() unless c_file.nil?
		h_file.close() unless h_file.nil?
		loadall_file.close() unless loadall_file.nil?
	end
end

def DumpUnprocessedExts
	if $extensionsToProcess and not $extensionsToProcess.empty?
		puts "The following extensions were in the filter file but were not found or they had no entry points:\n\n"
		
		$extensionsToProcess.each do |ext| 
			puts "\t#{ext}"
		end
	end
end


if ParseCmdLine()
	ParseFilterFile()
	GenerateSource()
	DumpUnprocessedExts()
	puts "Done."
end
