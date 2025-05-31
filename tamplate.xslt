<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"> 
<xsl:template match="/"> 
  <html> 
  <head> 
    <style> 
      body { 
        font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; 
        margin: 20px; 
      }
      table {  
        border-collapse: collapse;  
        width: 100%; 
        margin-top: 15px;
        box-shadow: 0 1px 3px rgba(0,0,0,0.1);
      } 
      th, td {  
        border: 1px solid #e0e0e0;  
        padding: 10px 12px;  
        text-align: left; 
      } 
      .false { 
        color: #d32f2f; 
        font-weight: bold;
      } 
      .true { 
        color: #388e3c; 
        font-weight: bold;
      } 
      .header { 
        background-color: #c8e6c9; /* 浅绿色背景 */
        color: #2e7d32; /* 深绿色文字 */
        font-weight: bold; 
        text-align: center;
      } 
      h2 {
        color: #2e7d32;
        border-bottom: 2px solid #c8e6c9;
        padding-bottom: 5px;
      }
      .summary {
        background-color: #f1f8e9;
        padding: 15px;
        border-radius: 5px;
        margin-top: 20px;
      }
      .summary p {
        margin: 5px 0;
      }
    </style> 
  </head> 
  <body> 
    <h2>📝 单元测试报告</h2> 
    <h3>问题反馈: jassimxiong@gmail.com</h3> 
    <table> 
      <tr class="header"> 
        <th>测试套件</th> 
        <th>测试用例</th> 
        <th>表达式</th> 
        <th>测试结果</th> 
      </tr> 
      <xsl:for-each select="doctest/TestSuite"> 
        <xsl:variable name="testSuite" select="@name"/> 
        <xsl:variable name="totalRows" select="count(TestCase/Expression) + count(TestCase[not(Expression)])"/> 
        <xsl:for-each select="TestCase"> 
          <xsl:variable name="testCase" select="@name"/> 
          <xsl:choose> 
            <xsl:when test="Expression"> 
              <xsl:for-each select="Expression"> 
                <tr> 
                  <xsl:if test="position() = 1 and count(../preceding-sibling::TestCase) = 0"> 
                    <td rowspan="{$totalRows}" style="background-color:#f1f8e9; vertical-align:middle; font-weight:bold;"> 
                      <xsl:value-of select="$testSuite"/> 
                    </td> 
                  </xsl:if> 
                  <td><xsl:value-of select="$testCase"/></td> 
                  <td><xsl:value-of select="Original"/></td> 
                  <td class="{@success}"><xsl:value-of select="@success"/></td> 
                </tr> 
              </xsl:for-each> 
            </xsl:when> 
            <xsl:otherwise> 
              <tr> 
                <xsl:if test="count(preceding-sibling::TestCase) = 0"> 
                  <td rowspan="{$totalRows}" style="background-color:#f1f8e9; vertical-align:middle; font-weight:bold;"> 
                    <xsl:value-of select="$testSuite"/> 
                  </td> 
                </xsl:if> 
                <td><xsl:value-of select="$testCase"/></td> 
                <td>N/A</td> 
                <td class="true">true</td> 
              </tr> 
            </xsl:otherwise> 
          </xsl:choose> 
        </xsl:for-each> 
      </xsl:for-each> 
    </table> 
    
    <div class="summary">
      <h2>📊 测试汇总</h2> 
      <p>✅ 成功用例总计: <xsl:value-of select="doctest/OverallResultsTestCases/@successes"/></p> 
      <p>❌ 失败用例总计: <xsl:value-of select="doctest/OverallResultsTestCases/@failures"/></p> 
      <p>✅ 成功断言总计: <xsl:value-of select="doctest/OverallResultsAsserts/@successes"/></p> 
      <p>❌ 失败断言总计: <xsl:value-of select="doctest/OverallResultsAsserts/@failures"/></p> 
    </div>
  </body> 
  </html> 
</xsl:template> 
</xsl:stylesheet>