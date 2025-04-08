import React, { useRef, useEffect } from 'react';
import PropTypes from 'prop-types';
import * as d3 from 'd3';

/**
 * D3.js를 사용한 파이 차트 컴포넌트
 * @param {Object} props
 * @param {Array} props.data - 차트 데이터
 * @param {string} props.nameKey - 이름 필드 키
 * @param {string} props.valueKey - 값 필드 키
 * @param {Array} props.colors - 색상 배열
 * @param {string} props.title - 차트 제목
 * @param {number} props.height - 차트 높이
 * @param {string} props.backgroundColor - 배경 색상
 * @param {string} props.textColor - 텍스트 색상
 * @returns {JSX.Element}
 */
const PieChartD3 = ({ 
  data, 
  nameKey, 
  valueKey, 
  colors = ['#0088FE', '#00C49F', '#FFBB28', '#FF8042', '#8884d8', '#82ca9d'],
  title,
  height = 300,
  backgroundColor = '#0c1e36',
  textColor = '#ffffff'
}) => {
  const svgRef = useRef(null);
  
  useEffect(() => {
    if (!data || data.length === 0) return;
    
    // SVG 컨테이너와 크기 정의
    const svg = d3.select(svgRef.current);
    svg.selectAll("*").remove(); // 이전 차트 제거
    
    const width = svgRef.current.clientWidth;
    const radius = Math.min(width, height) / 2 * 0.8;
    
    // 메인 그래프 영역 생성
    const chart = svg
      .append('g')
      .attr('transform', `translate(${width / 2}, ${height / 2})`);
    
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
    
    // 색상 스케일 설정
    const color = d3.scaleOrdinal()
      .domain(data.map(d => d[nameKey]))
      .range(colors);
    
    // 파이 생성기 정의
    const pie = d3.pie()
      .sort(null)
      .value(d => d[valueKey]);
    
    // 아크 생성기 정의
    const arc = d3.arc()
      .innerRadius(radius * 0.5) // 도넛 차트로 변경
      .outerRadius(radius);
    
    // 레이블을 위한 아크 생성기
    const labelArc = d3.arc()
      .innerRadius(radius * 0.75)
      .outerRadius(radius * 0.75);
    
    // 중앙 텍스트를 위한 원 추가
    chart.append('circle')
      .attr('cx', 0)
      .attr('cy', 0)
      .attr('r', radius * 0.48)
      .attr('fill', backgroundColor)
      .attr('stroke', '#1e3a5a')
      .attr('stroke-width', 1);
      
    // 중앙 텍스트 추가
    const centerText = chart.append('text')
      .attr('class', 'center-text')
      .attr('text-anchor', 'middle')
      .attr('dominant-baseline', 'middle')
      .attr('fill', textColor);
      
    centerText.append('tspan')
      .attr('class', 'center-total')
      .attr('x', 0)
      .attr('dy', '-0.5em')
      .attr('font-size', '24px')
      .text(d3.sum(data, d => d[valueKey]));
      
    centerText.append('tspan')
      .attr('class', 'center-label')
      .attr('x', 0)
      .attr('dy', '1.5em')
      .attr('font-size', '14px')
      .text('총 개수');
    
    // 파이 데이터 생성
    const arcs = chart.selectAll('.arc')
      .data(pie(data))
      .enter()
      .append('g')
      .attr('class', 'arc');
    
    // 각 아크 그리기
    arcs.append('path')
      .attr('d', arc)
      .attr('fill', d => color(d.data[nameKey]))
      .attr('stroke', '#ffffff')
      .attr('stroke-width', 1)
      .style('opacity', 0.8)
      .transition() // 애니메이션 추가
      .duration(1000)
      .attrTween('d', function(d) {
        const i = d3.interpolate({startAngle: 0, endAngle: 0}, d);
        return function(t) {
          return arc(i(t));
        };
      });
      
    // 호버 효과를 위한 아크 생성기
    const arcHover = d3.arc()
      .innerRadius(radius * 0.45)
      .outerRadius(radius * 1.05);
      
    // 각 아크에 대한 인터랙션
    arcs.selectAll('path')
      .on('mouseover', function(event, d) {
        // 슬라이스 확장 효과
        d3.select(this)
          .transition()
          .duration(200)
          .attr('d', arcHover)
          .style('opacity', 1);
          
        // 중앙 텍스트 업데이트
        const percent = (d.data[valueKey] / d3.sum(data, d => d[valueKey]) * 100).toFixed(1);
        centerText.select('.center-total')
          .text(d.data[valueKey]);
        centerText.select('.center-label')
          .text(`${d.data[nameKey]} (${percent}%)`);
        
        tooltip
          .style('visibility', 'visible')
          .html(`${d.data[nameKey]}: ${d.data[valueKey]} (${percent}%)`);
      })
      .on('mousemove', function(event) {
        tooltip
          .style('top', (event.pageY - 10) + 'px')
          .style('left', (event.pageX + 10) + 'px');
      })
      .on('mouseout', function() {
        // 슬라이스 원래 크기로
        d3.select(this)
          .transition()
          .duration(200)
          .attr('d', arc)
          .style('opacity', 0.8);
          
        // 중앙 텍스트 초기화
        centerText.select('.center-total')
          .text(d3.sum(data, d => d[valueKey]));
        centerText.select('.center-label')
          .text('총 개수');
          
        tooltip.style('visibility', 'hidden');
      });
    
    // 제목 추가
    if (title) {
      svg.append('text')
        .attr('x', width / 2)
        .attr('y', 20)
        .attr('text-anchor', 'middle')
        .style('font-size', '16px')
        .style('fill', textColor)
        .text(title);
    }
    
    // 범례 추가
    const legend = svg.append('g')
      .attr('font-family', 'sans-serif')
      .attr('font-size', 10)
      .attr('text-anchor', 'end')
      .selectAll('g')
      .data(data)
      .enter().append('g')
      .attr('transform', (d, i) => `translate(${width - 10}, ${i * 20 + 20})`);

    legend.append('rect')
      .attr('x', -19)
      .attr('width', 19)
      .attr('height', 19)
      .attr('fill', d => color(d[nameKey]));

    legend.append('text')
      .attr('x', -24)
      .attr('y', 9.5)
      .attr('dy', '0.32em')
      .attr('fill', textColor)
      .text(d => {
        // 이름이 너무 길면 잘라서 표시
        const name = d[nameKey];
        return name.length > 15 ? name.substr(0, 12) + '...' : name;
      });
    
    // 정리 함수
    return () => {
      tooltip.remove();
    };
  }, [data, nameKey, valueKey, colors, title, height, backgroundColor, textColor]);
  
  return (
    <div style={{ width: '100%', height: height, backgroundColor: backgroundColor }}>
      <svg ref={svgRef} width="100%" height={height}></svg>
    </div>
  );
};

PieChartD3.propTypes = {
  data: PropTypes.array.isRequired,
  nameKey: PropTypes.string.isRequired,
  valueKey: PropTypes.string.isRequired,
  colors: PropTypes.array,
  title: PropTypes.string,
  height: PropTypes.number,
  backgroundColor: PropTypes.string,
  textColor: PropTypes.string
};

export default PieChartD3; 