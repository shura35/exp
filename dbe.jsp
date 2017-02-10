<%@ page language="java" import="java.sql.*" %>

<html>
  <body>

<% String searchCondition = request.getParameter("sql"); 
   if (searchCondition != null) { %>
      <h3> Search results for  <i> <%= searchCondition %> </i> </h3>
      <b> <%= runQuery(searchCondition) %> </b> <hr><br>
<% }  %>

    <form method="get" action=""> 
      <input type="text" name="sql" SIZE=30>
      <input type="submit" value="Ask Oracle");
    </form>
   </body>
</html>

<%! private String runQuery(String sql) throws SQLException {
     Connection conn = null;
     Statement stmt = null;
     ResultSet rset = null;
     
     try {
        DriverManager.registerDriver(new oracle.jdbc.driver.OracleDriver());
        conn = DriverManager.getConnection("jdbc:oracle:thin:@vcs47v05.dc.uro.equant.com:1521:PRDEGN",
                                           "ist_rules_user", "ist_rules_user");
        stmt = conn.createStatement();
        rset = stmt.executeQuery (sql);
        return (formatResult(rset));
     } catch (SQLException e) { 
         return ("<p> SQL error: <pre> " + e + " </pre> </p>\n");
     } finally {
         if (rset!= null) rset.close(); 
         if (stmt!= null) stmt.close();
         if (conn!= null) conn.close();
     }
  }
  private String formatResult(ResultSet rset) throws SQLException {
    StringBuffer sb = new StringBuffer();
    if (!rset.next())
      sb.append("<p> No matching rows.<p>\n");
    else {  sb.append("<ul>"); 
            do {  sb.append("<li>" + rset.getString(1) + 
                            " earns $ " + rset.getInt(2) + ".</li>\n");
            } while (rset.next());
           sb.append("</ul>"); 
    }
    return sb.toString();
  }
%>
