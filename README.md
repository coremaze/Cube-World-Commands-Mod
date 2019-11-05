# Cube-World-Commands-Mod
For use with the Cube World Mod Launcher: https://github.com/ChrisMiuchiz/Cube-World-Mod-Launcher

This mod implements additional commands into Cube World. It also adds the ability to use the arrow keys to get messages you have previously sent, like how most command lines work.

## Commands:
 #### Get help
  
    /help [page number]

 ---
 
 #### Display world coordinates
  
    /coords
    
 ---
 
 #### Teleport in terms of map coordinates

    /tp <x> <y>
 
 ---
 
 #### Teleport to another player

    /tp <name>
 
 ---
 
 #### Set time
 
    /settime <hour>:<minute>
    
 ---
 
 #### Set name
 
    /name <name>
    
 ---
 
 #### Connect to a server via steam ID
 
    /join <Steam ID>
    
 ---
 
 #### Connect to a server using an alias, specified in Mods\CommandsMod\join.txt
 
    /join <alias>
    
 ---
 
 #### Allow anyone to connect to your server
 
    /server open
    
 ---
 
 #### Stop allowing anyone to connect to your server
 
    /server close
    
 ---
 
 #### Refuse all new sessions
 
    /server block
    
 ---
 
 #### Enable blacklisting Steam IDs specified in Mods\CommandsMod\blacklist.txt
 
    /server blacklist
    
 ---
 
 #### Log new session requests
 
    /server log
    
 ---
 
 #### Teleport to your cursor position on the map
 
    /tpmap
	
 ---
 
 #### Create a waypoint/home you can teleport to 
 
    /sethome <alias>
	
 ---
 
 #### Teleport to a waypoint/home
 
    /home <alias>
	
 ---

 #### Change chat size
 
    /gui chat <x size> <y size>
	
 ---
 
 #### Use a skill - This will also tell you the identifier of the skill, if available.
 
    /skill <id>
    
    