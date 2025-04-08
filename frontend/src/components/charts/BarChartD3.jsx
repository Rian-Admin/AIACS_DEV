import React, { useRef, useEffect } from 'react';
import PropTypes from 'prop-types';
import * as d3 from 'd3';

/**
 * D3.js를 사용한 바 차트 컴포넌트
 * @param {Object} props
 * @param {Array} props.data - 차트 데이터
 * @param {string} props.xKey - X축 데이터 키
 * @param {string} props.yKey - Y축 데이터 키
 * @param {string} props.xLabel - X축 레이블
 * @param {string} props.yLabel - Y축 레이블
 * @param {string} props.color - 바 색상
 * @param {string} props.tooltipLabel - 툴팁 레이블
 * @param {number} props.height - 차트 높이
 * @param {string} props.backgroundColor - 배경 색상
 * @param {string} props.textColor - 텍스트 색상
 * @param {string} props.gridColor - 그리드 색상
 * @returns {JSX.Element}
 */
const BarChartD3 = ({ 
  data, 
  xKey, 
  yKey, 
  xLabel, 
  yLabel, 
  color = '#0088FE', 
  tooltipLabel,
  height = 300,
  backgroundColor = '#0c1e36',
  textColor = '#ffffff',
  gridColor = '#1e3a5a'
}) => {
  const svgRef = useRef(null);
  
  useEffect(() => {
    if (!data || data.length === 0) return;
    
    // SVG 컨테이너와 크기 정의
    const svg = d3.select(svgRef.current);
    svg.selectAll("*").remove(); // 이전 차트 제거
    
    const margin = { top: 30, right: 30, bottom: 50, left: 60 };
    const width = svgRef.current.clientWidth - margin.left - margin.right;
    const chartHeight = height - margin.top - margin.bottom;
    
    // 메인 그래프 영역 생성
    const chart = svg
      .append('g')
      .attr('transform', `translate(${margin.left}, ${margin.top})`);
    
    // 툴팁 생성
    const tooltip = d3.select('body')
      .append('div')
      .attr('class', 'd3-tooltip')
      .style('position', 'absolute')
      .style('visibility', 'hidden')
      .style('background-color', '#0a1929')
      .style('border', '1px solid #1e3a5a')
      .style('border-radius', '4px')
      .style('padding', '8px')
      .style('color', '#ffffff')
      .style('pointer-events', 'none')
      .style('z-index', 1000);
    
    // X 스케일 설정
    const x = d3.scaleBand()
      .domain(data.map(d => d[xKey]))
      .range([0, width])
      .padding(0.3);
    
    // Y 스케일 설정
    const y = d3.scaleLinear()
      .domain([0, d3.max(data, d => d[yKey]) * 1.1]) // 최대값의 10% 마진 추가
      .range([chartHeight, 0]);
    
    // X 축 생성
    chart.append('g')
      .attr('transform', `translate(0, ${chartHeight})`)
      .call(d3.axisBottom(x))
      .call(g => g.select('.domain').attr('stroke', textColor))
      .call(g => g.selectAll('.tick line').attr('stroke', textColor))
      .call(g => g.selectAll('.tick text').attr('fill', textColor))
      .selectAll('text')
      .style('text-anchor', 'end')
      .attr('transform', 'rotate(-45)')
      .attr('dx', '-.8em')
      .attr('dy', '.15em');
    
    // Y 축 생성
    chart.append('g')
      .call(d3.axisLeft(y))
      .call(g => g.select('.domain').attr('stroke', textColor))
      .call(g => g.selectAll('.tick line').attr('stroke', textColor))
      .call(g => g.selectAll('.tick text').attr('fill', textColor));
    
    // 그리드 라인 추가
    chart.append('g')
      .attr('class', 'grid')
      .call(d3.axisLeft(y)
        .tickSize(-width)
        .tickFormat(''))
      .call(g => g.select('.domain').remove())
      .call(g => g.selectAll('.tick line')
        .attr('stroke', gridColor)
        .attr('stroke-dasharray', '3,3')
        .attr('stroke-opacity', 0.7));
    
    // 바 생성
    chart.selectAll('.bar')
      .data(data)
      .enter()
      .append('rect')
      .attr('class', 'bar')
      .attr('x', d => x(d[xKey]))
      .attr('width', x.bandwidth())
      .attr('fill', (d, i) => {
        // 그라데이션 생성
        const gradientId = `bar-gradient-${i}`;
        const gradient = chart.append('linearGradient')
          .attr('id', gradientId)
          .attr('gradientUnits', 'userSpaceOnUse')
          .attr('x1', 0)
          .attr('y1', chartHeight)
          .attr('x2', 0)
          .attr('y2', 0);
        
        gradient.append('stop')
          .attr('offset', '0%')
          .attr('stop-color', d3.color(color).darker(0.5));
        
        gradient.append('stop')
          .attr('offset', '100%')
          .attr('stop-color', d3.color(color).brighter(0.2));
        
        return `url(#${gradientId})`;
      })
      // 애니메이션 시작 위치
      .attr('y', chartHeight)
      .attr('height', 0)
      // 테두리 추가
      .attr('stroke', d3.color(color).darker(0.7))
      .attr('stroke-width', 1)
      .attr('rx', 3) // 모서리 둥글게
      .attr('ry', 3)
      .transition() // 성장 애니메이션
      .duration(800)
      .delay((d, i) => i * 50)
      .attr('y', d => y(d[yKey]))
      .attr('height', d => chartHeight - y(d[yKey]));

    // 인터랙션 및 툴팁
    chart.selectAll('.bar-interaction')
      .data(data)
      .enter()
      .append('rect')
      .attr('class', 'bar-interaction')
      .attr('x', d => x(d[xKey]))
      .attr('y', d => y(d[yKey]))
      .attr('width', x.bandwidth())
      .attr('height', d => chartHeight - y(d[yKey]))
      .attr('fill', 'transparent')
      .on('mouseover', function(event, d) {
        // 해당 바 강조 효과
        const barIndex = data.indexOf(d);
        const barElement = chart.selectAll('.bar').nodes()[barIndex];
        
        d3.select(barElement)
          .transition()
          .duration(200)
          .attr('fill', d3.color(color).brighter(0.7));
        
        // 값 레이블 표시
        chart.append('text')
          .attr('class', 'temp-value')
          .attr('x', x(d[xKey]) + x.bandwidth() / 2)
          .attr('y', y(d[yKey]) - 10)
          .attr('text-anchor', 'middle')
          .attr('fill', textColor)
          .text(`${d[yKey]}${tooltipLabel || ''}`);
          
        // 툴팁 표시
        tooltip
          .style('visibility', 'visible')
          .html(`
            <div style="font-weight: bold; margin-bottom: 5px;">${d[xKey]}</div>
            <div style="display: flex; align-items: center;">
              <div style="width: 10px; height: 10px; background-color: ${color}; margin-right: 5px;"></div>
              <div>${d[yKey]} ${tooltipLabel || ''}</div>
            </div>
            ${d.count !== undefined ? `<div style="margin-top: 3px;">총 횟수: ${d.count}</div>` : ''}
          `);
      })
      .on('mousemove', function(event) {
        tooltip
          .style('top', (event.pageY - 10) + 'px')
          .style('left', (event.pageX + 10) + 'px');
      })
      .on('mouseout', function(event, d) {
        // 바 원래 색상으로 복원
        const barIndex = data.indexOf(d);
        const barElement = chart.selectAll('.bar').nodes()[barIndex];
        const gradientId = `bar-gradient-${barIndex}`;
        
        d3.select(barElement)
          .transition()
          .duration(200)
          .attr('fill', `url(#${gradientId})`);
        
        // 임시 값 레이블 제거
        chart.selectAll('.temp-value').remove();
        
        // 툴팁 숨기기
        tooltip.style('visibility', 'hidden');
      });
    
    // X 축 레이블 추가
    chart.append('text')
      .attr('class', 'x-label')
      .attr('text-anchor', 'middle')
      .attr('x', width / 2)
      .attr('y', chartHeight + margin.bottom - 5)
      .attr('fill', textColor)
      .text(xLabel);
    
    // Y 축 레이블 추가
    chart.append('text')
      .attr('class', 'y-label')
      .attr('text-anchor', 'middle')
      .attr('transform', `translate(${-margin.left / 1.5}, ${chartHeight / 2}) rotate(-90)`)
      .attr('fill', textColor)
      .text(yLabel);
    
    // 정리 함수
    return () => {
      tooltip.remove();
    };
  }, [data, xKey, yKey, xLabel, yLabel, color, tooltipLabel, height, backgroundColor, textColor, gridColor]);
  
  return (
    <div style={{ width: '100%', height: height, backgroundColor: backgroundColor }}>
      <svg ref={svgRef} width="100%" height={height}></svg>
    </div>
  );
};

BarChartD3.propTypes = {
  data: PropTypes.array.isRequired,
  xKey: PropTypes.string.isRequired,
  yKey: PropTypes.string.isRequired,
  xLabel: PropTypes.string,
  yLabel: PropTypes.string,
  color: PropTypes.string,
  tooltipLabel: PropTypes.string,
  height: PropTypes.number,
  backgroundColor: PropTypes.string,
  textColor: PropTypes.string,
  gridColor: PropTypes.string
};

export default BarChartD3; 