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
      .summary-grid {
        display: grid;
        grid-template-columns: repeat(2, 1fr);
        gap: 10px;
      }
      .summary-item {
        background: white;
        padding: 10px;
        border-radius: 4px;
        box-shadow: 0 1px 2px rgba(0,0,0,0.05);
      }
      .summary p {
        margin: 5px 0;
      }
      .stats-label {
        font-weight: 600;
        color: #2e7d32;
      }
      .stats-value {
        font-weight: bold;
        font-size: 1.1em;
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
      
      <!-- 基础统计信息 -->
      <div class="summary-grid">
        <div class="summary-item">
          <p><span class="stats-label">测试套件总数：</span></p>
          <p class="stats-value"><xsl:value-of select="count(doctest/TestSuite)"/></p>
        </div>
        
        <div class="summary-item">
          <p><span class="stats-label">测试用例总数：</span></p>
          <p class="stats-value"><xsl:value-of select="count(//TestCase)"/></p>
        </div>
        
        <div class="summary-item">
          <p><span class="stats-label">成功用例：</span></p>
          <p class="stats-value" style="color:#388e3c">✅ <xsl:value-of select="doctest/OverallResultsTestCases/@successes"/></p>
        </div>
        
        <div class="summary-item">
          <p><span class="stats-label">失败用例：</span></p>
          <p class="stats-value" style="color:#d32f2f">❌ <xsl:value-of select="doctest/OverallResultsTestCases/@failures"/></p>
        </div>
      </div>
      
      <!-- 详细统计 -->
      <div style="margin-top:20px">
        <div class="summary-grid">
          <div class="summary-item">
            <p><span class="stats-label">成功断言：</span></p>
            <p class="stats-value" style="color:#388e3c">✅ <xsl:value-of select="doctest/OverallResultsAsserts/@successes"/></p>
          </div>
          
          <div class="summary-item">
            <p><span class="stats-label">失败断言：</span></p>
            <p class="stats-value" style="color:#d32f2f">❌ <xsl:value-of select="doctest/OverallResultsAsserts/@failures"/></p>
          </div>
          
          <div class="summary-item">
            <p><span class="stats-label">总断言数：</span></p>
            <p class="stats-value"><xsl:value-of select="doctest/OverallResultsAsserts/@successes + doctest/OverallResultsAsserts/@failures"/></p>
          </div>
          
          <div class="summary-item">
            <p><span class="stats-label">通过率：</span></p>
            <p class="stats-value">
              <xsl:variable name="totalAsserts" select="doctest/OverallResultsAsserts/@successes + doctest/OverallResultsAsserts/@failures"/>
              <xsl:variable name="passRate" select="format-number(doctest/OverallResultsAsserts/@successes div $totalAsserts * 100, '0.00')"/>
              <xsl:value-of select="$passRate"/>%
            </p>
          </div>
        </div>
      </div>
    </div>
  </body> 
  </html> 
</xsl:template> 
</xsl:stylesheet>