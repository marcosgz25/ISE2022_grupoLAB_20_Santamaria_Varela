t <html><head><title>Ganancia</title>
t <script language=JavaScript type="text/javascript" src="xml_http.js"></script>
t <script language=JavaScript type="text/javascript">
t </script></head>
i pg_header.inc
t <h2 align="center"><br>Ganancia</h2>
t <p><font size="2">This page allows you to change the Instrumental amplifier
t  ganancy.
t <form action=ganancia.cgi method=post name=form1>
t <input type=hidden value="led" name=pg>
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#aaccff>
t  <th width=40%>Item</th>
t  <th width=60%>Setting</th></tr>
t <td><img src=pabb.gif>Valores de ganancia:</td>
t <td><select name="ctrl" onchange="submit();">
c b c <option %s>Ganancia 1</option><option %s>Ganancia 50</option>
c b c <option %s>Ganancia 10</option><option %s>Ganancia 5</option>
c b c <option %s>Ganancia 100</option></select></td></tr>
# Here begin button definitions
t <p align=center>
t <input type=submit name=set value="Send" id="sbm">
t <input type=reset value="Undo">
t </p></form>
i pg_footer.inc
. End of script must be closed with period.
