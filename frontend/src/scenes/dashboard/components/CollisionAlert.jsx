import { Box, Typography, Paper, List, ListItem, ListItemText, ListItemAvatar, Avatar } from '@mui/material';
import { translate, translateSpecies } from '../../../utils/i18n';
import { Warning as WarningIcon } from '@mui/icons-material';

const CollisionAlert = ({ collisionRisks, language }) => {
  // 위험 레벨에 따른 색상 반환
  const getRiskColor = (level) => {
    switch (level) {
      case 'critical':
        return '#d32f2f';
      case 'high':
        return '#f57c00';
      case 'medium':
        return '#fbc02d';
      case 'low':
      default:
        return '#388e3c';
    }
  };

  return (
    <Paper 
      elevation={0} 
      sx={{ 
        p: 2, 
        borderRadius: 2, 
        backgroundColor: '#0a1929',
        border: '1px solid #1e3a5a',
      }}
    >
      <Typography variant="h6" fontWeight="bold" sx={{ mb: 1 }}>
        {translate('충돌 위험 알림', 'Collision Risk Alerts', language)}
      </Typography>
      
      {collisionRisks.length > 0 ? (
        <List sx={{ p: 0 }}>
          {collisionRisks.map((risk) => (
            <ListItem 
              key={risk.id} 
              sx={{ 
                px: 0, 
                py: 1, 
                borderLeft: `4px solid ${getRiskColor(risk.level)}`,
                pl: 1,
                backgroundColor: `${getRiskColor(risk.level)}10`,
                mb: 1,
                borderRadius: '0 4px 4px 0'
              }}
            >
              <ListItemAvatar sx={{ minWidth: 40 }}>
                <Avatar 
                  sx={{ 
                    width: 32, 
                    height: 32,
                    bgcolor: `${getRiskColor(risk.level)}40`,
                    color: getRiskColor(risk.level)
                  }}
                >
                  <WarningIcon fontSize="small" />
                </Avatar>
              </ListItemAvatar>
              <ListItemText 
                primary={
                  <Typography variant="body2" fontWeight="bold">
                    {risk.location} - {translateSpecies(risk.species, language)}
                  </Typography>
                }
                secondary={
                  <Typography variant="caption" color="text.secondary">
                    {translate('거리', 'Distance', language)}: {risk.distance}m | 
                    {translate('속도', 'Speed', language)}: {risk.speed}km/h
                  </Typography>
                }
              />
              <Typography variant="caption" color="text.secondary">
                {new Date(risk.timestamp).toLocaleTimeString()}
              </Typography>
            </ListItem>
          ))}
        </List>
      ) : (
        <Box 
          sx={{ 
            display: 'flex', 
            flexDirection: 'column', 
            alignItems: 'center', 
            justifyContent: 'center',
            py: 2
          }}
        >
          <Typography variant="body2" color="text.secondary" align="center">
            {translate('현재 충돌 위험 없음', 'No collision risks detected', language)}
          </Typography>
        </Box>
      )}
    </Paper>
  );
};

export default CollisionAlert; 