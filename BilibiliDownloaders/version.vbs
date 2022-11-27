Function RegExpReplease(pattern, src, change)
    Dim regEx, Match, Matches
    set regEx = new RegExp
    regEx.Pattern = pattern
    regEx.IgnoreCase = True
    regEx.Global = True
    Set Matches = regEx.Execute(src)

    For Each Match in Matches
        allItems = Match.Value
        If Match.SubMatches.count = 2 then
            result = Match.SubMatches.Item(1)
            allResult = replace(allItems, Match.SubMatches.Item(1), change)
            src = replace(src, allItems, allResult)
        End If
    Next
        RegExpReplease = src
End Function

pattern_yyyy = "(BUILD_YEAR[\s]*(\d+))" 
pattern_tt = "(BUILD_MONTH[\s]*(\d+))" 
pattern_dd = "(BUILD_DAY[\s]*(\d+))" 
pattern_hh = "(BUILD_HOUR[\s]*(\d+))" 
pattern_MM = "(BUILD_MINUTE[\s]*(\d+))" 
pattern_ss = "(BUILD_SECOND[\s]*(\d+))" 
pattern_hash = "(GIT_HASH[\s]*([A-Za-z0-9]+))" 

str_yy = year(now)
str_yy = right(str_yy, len(str_yy) - 2)
str_tt = Month(now)
if len(str_tt) = 1 then str_tt = "0"&str_tt
str_dd = Day(now)
if len(str_dd) = 1 then str_dd = "0"&str_dd
str_hh = Hour(now)
if len(str_hh) = 1 then str_hh = "0"&str_hh
str_MM = Minute(now)
if len(str_MM) = 1 then str_MM = "0"&str_MM
str_ss = Second(now)
if len(str_ss) = 1 then str_ss = "0"&str_ss



set fs = wscript.CreateObject("Scripting.FileSystemObject")
set ts_hash = fs.OpenTextFile("temp_hash.txt", 1)
str_hash = ts_hash.readline()
str_hash = Left(str_hash, 9)
ts_hash.close

set ts_content = fs.OpenTextFile("src/AppVersion.h", 1)
content = ts_content.readall()
ts_content.close

content = RegExpReplease(pattern_yy, content, str_yy)
content = RegExpReplease(pattern_tt, content, str_tt)
content = RegExpReplease(pattern_dd, content, str_dd)
content = RegExpReplease(pattern_hh, content, str_hh)
content = RegExpReplease(pattern_MM, content, str_MM)
content = RegExpReplease(pattern_ss, content, str_ss)
content = RegExpReplease(pattern_hash, content, str_hash)

set ts_save = fs.opentextfile("temp_version.h", 2, True)
ts_save.write(content)
ts_save.close
