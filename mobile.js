// App.js - Mobile Banking Frontend
import React, { useState, useEffect } from 'react';
import { 
  View, 
  Text, 
  StyleSheet, 
  TextInput, 
  TouchableOpacity, 
  ScrollView, 
  RefreshControl, 
  Alert, 
  Modal, 
  Image,
  ActivityIndicator,
  FlatList,
  SafeAreaView
} from 'react-native';
import { MaterialIcons, FontAwesome5 } from '@expo/vector-icons';
import * as ImagePicker from 'expo-image-picker';
import axios from 'axios';

// API Configuration
const API_URL = 'http://localhost:5000/api';

// Create axios instance
const api = axios.create({
  baseURL: API_URL,
  headers: {
    'Content-Type': 'application/json',
  },
});

// Request interceptor to add auth token
api.interceptors.request.use(
  (config) => {
    const token = localStorage.getItem('token');
    if (token) {
      config.headers.Authorization = `Bearer ${token}`;
    }
    return config;
  },
  (error) => {
    return Promise.reject(error);
  }
);

// Main App Component
export default function App() {
  const [user, setUser] = useState(null);
  const [token, setToken] = useState(null);
  const [loading, setLoading] = useState(false);
  const [refreshing, setRefreshing] = useState(false);
  const [dashboardData, setDashboardData] = useState(null);
  const [transactions, setTransactions] = useState([]);
  const [savings, setSavings] = useState([]);
  
  // Auth states
  const [phone, setPhone] = useState('');
  const [pin, setPin] = useState('');
  const [name, setName] = useState('');
  const [showLogin, setShowLogin] = useState(true);
  
  // Transaction states
  const [recipientPhone, setRecipientPhone] = useState('');
  const [amount, setAmount] = useState('');
  const [description, setDescription] = useState('');
  const [showSendMoney, setShowSendMoney] = useState(false);
  
  // KYC states
  const [showKYC, setShowKYC] = useState(false);
  const [kycDocuments, setKycDocuments] = useState([]);
  
  // QR states
  const [showQR, setShowQR] = useState(false);
  const [qrCode, setQrCode] = useState(null);
  
  // Savings states
  const [showSavings, setShowSavings] = useState(false);
  const [savingsType, setSavingsType] = useState('flexi');
  const [savingsName, setSavingsName] = useState('');
  const [targetAmount, setTargetAmount] = useState('');
  const [interestRate, setInterestRate] = useState('');

  // Check for existing token on app start
  useEffect(() => {
    const storedToken = localStorage.getItem('token');
    if (storedToken) {
      setToken(storedToken);
      fetchUserProfile();
      fetchDashboardData();
    }
  }, []);

  // Fetch user profile
  const fetchUserProfile = async () => {
    try {
      const response = await api.get('/users/profile');
      setUser(response.data);
    } catch (error) {
      console.error('Error fetching user profile:', error);
      logout();
    }
  };

  // Fetch dashboard data
  const fetchDashboardData = async () => {
    try {
      const response = await api.get('/dashboard');
      setDashboardData(response.data);
    } catch (error) {
      console.error('Error fetching dashboard data:', error);
    }
  };

  // Fetch transactions
  const fetchTransactions = async () => {
    try {
      const response = await api.get('/transactions/history');
      setTransactions(response.data.transactions);
    } catch (error) {
      console.error('Error fetching transactions:', error);
    }
  };

  // Fetch savings
  const fetchSavings = async () => {
    try {
      const response = await api.get('/savings');
      setSavings(response.data);
    } catch (error) {
      console.error('Error fetching savings:', error);
    }
  };

  // Handle login
  const handleLogin = async () => {
    if (!phone || !pin) {
      Alert.alert('Error', 'Please enter phone number and PIN');
      return;
    }

    setLoading(true);
    try {
      const response = await api.post('/auth/login', { phone, pin });
      const { token } = response.data;
      setToken(token);
      localStorage.setItem('token', token);
      fetchUserProfile();
      fetchDashboardData();
      setPhone('');
      setPin('');
    } catch (error) {
      Alert.alert('Login Failed', error.response?.data?.message || 'Invalid credentials');
    } finally {
      setLoading(false);
    }
  };

  // Handle registration
  const handleRegister = async () => {
    if (!phone || !pin || !name) {
      Alert.alert('Error', 'Please fill all fields');
      return;
    }

    setLoading(true);
    try {
      const response = await api.post('/auth/register', { phone, pin, name });
      const { token } = response.data;
      setToken(token);
      localStorage.setItem('token', token);
      fetchUserProfile();
      fetchDashboardData();
      setPhone('');
      setPin('');
      setName('');
    } catch (error) {
      Alert.alert('Registration Failed', error.response?.data?.message || 'Registration failed');
    } finally {
      setLoading(false);
    }
  };

  // Handle logout
  const logout = () => {
    setUser(null);
    setToken(null);
    localStorage.removeItem('token');
    setDashboardData(null);
    setTransactions([]);
    setSavings([]);
  };

  // Handle send money
  const handleSendMoney = async () => {
    if (!recipientPhone || !amount) {
      Alert.alert('Error', 'Please fill all required fields');
      return;
    }

    if (isNaN(amount) || parseFloat(amount) <= 0) {
      Alert.alert('Error', 'Please enter a valid amount');
      return;
    }

    setLoading(true);
    try {
      const response = await api.post('/transactions/send-money', {
        recipientPhone,
        amount: parseFloat(amount),
        description
      });
      Alert.alert('Success', `Money sent successfully! Fee: ৳${response.data.fee.toFixed(2)}`);
      setShowSendMoney(false);
      setRecipientPhone('');
      setAmount('');
      setDescription('');
      fetchDashboardData();
      fetchTransactions();
    } catch (error) {
      Alert.alert('Error', error.response?.data?.message || 'Failed to send money');
    } finally {
      setLoading(false);
    }
  };

  // Handle KYC document upload
  const pickDocument = async () => {
    const result = await ImagePicker.launchImageLibraryAsync({
      mediaTypes: ImagePicker.MediaTypeOptions.Images,
      allowsEditing: true,
      aspect: [4, 3],
      quality: 1,
    });

    if (!result.cancelled) {
      setKycDocuments([...kycDocuments, result.uri]);
    }
  };

  // Submit KYC documents
  const submitKYC = async () => {
    if (kycDocuments.length === 0) {
      Alert.alert('Error', 'Please upload at least one document');
      return;
    }

    setLoading(true);
    try {
      const formData = new FormData();
      kycDocuments.forEach((doc, index) => {
        formData.append('documents', {
          uri: doc,
          type: 'image/jpeg',
          name: `document_${index}.jpg`,
        });
      });

      await api.post('/users/kyc', formData, {
        headers: {
          'Content-Type': 'multipart/form-data',
        },
      });
      
      Alert.alert('Success', 'KYC documents submitted successfully');
      setShowKYC(false);
      setKycDocuments([]);
      fetchUserProfile();
    } catch (error) {
      Alert.alert('Error', error.response?.data?.message || 'Failed to submit KYC');
    } finally {
      setLoading(false);
    }
  };

  // Generate QR code
  const generateQRCode = async () => {
    setLoading(true);
    try {
      const response = await api.get('/qr/user');
      setQrCode(response.data.qrCode);
      setShowQR(true);
    } catch (error) {
      Alert.alert('Error', 'Failed to generate QR code');
    } finally {
      setLoading(false);
    }
  };

  // Create savings account
  const createSavings = async () => {
    if (!savingsName || !targetAmount || !interestRate) {
      Alert.alert('Error', 'Please fill all fields');
      return;
    }

    setLoading(true);
    try {
      await api.post('/savings/create', {
        type: savingsType,
        name: savingsName,
        targetAmount: parseFloat(targetAmount),
        interestRate: parseFloat(interestRate)
      });
      
      Alert.alert('Success', 'Savings account created successfully');
      setShowSavings(false);
      setSavingsName('');
      setTargetAmount('');
      setInterestRate('');
      fetchSavings();
    } catch (error) {
      Alert.alert('Error', error.response?.data?.message || 'Failed to create savings');
    } finally {
      setLoading(false);
    }
  };

  // Refresh data
  const onRefresh = async () => {
    setRefreshing(true);
    await Promise.all([
      fetchUserProfile(),
      fetchDashboardData(),
      fetchTransactions(),
      fetchSavings()
    ]);
    setRefreshing(false);
  };

  // Render login/register screen
  const renderAuthScreen = () => (
    <SafeAreaView style={styles.container}>
      <View style={styles.authContainer}>
        <Text style={styles.title}>Mobile Banking</Text>
        
        <View style={styles.toggleContainer}>
          <TouchableOpacity 
            style={[styles.toggleButton, showLogin && styles.activeToggle]} 
            onPress={() => setShowLogin(true)}
          >
            <Text style={styles.toggleText}>Login</Text>
          </TouchableOpacity>
          <TouchableOpacity 
            style={[styles.toggleButton, !showLogin && styles.activeToggle]} 
            onPress={() => setShowLogin(false)}
          >
            <Text style={styles.toggleText}>Register</Text>
          </TouchableOpacity>
        </View>
        
        <TextInput
          style={styles.input}
          placeholder="Phone Number"
          value={phone}
          onChangeText={setPhone}
          keyboardType="phone-pad"
          autoCapitalize="none"
        />
        
        <TextInput
          style={styles.input}
          placeholder="PIN"
          value={pin}
          onChangeText={setPin}
          secureTextEntry
          keyboardType="numeric"
          maxLength={6}
        />
        
        {!showLogin && (
          <TextInput
            style={styles.input}
            placeholder="Full Name"
            value={name}
            onChangeText={setName}
          />
        )}
        
        <TouchableOpacity 
          style={styles.button} 
          onPress={showLogin ? handleLogin : handleRegister}
          disabled={loading}
        >
          {loading ? (
            <ActivityIndicator color="white" />
          ) : (
            <Text style={styles.buttonText}>{showLogin ? 'Login' : 'Register'}</Text>
          )}
        </TouchableOpacity>
      </View>
    </SafeAreaView>
  );

  // Render dashboard
  const renderDashboard = () => (
    <SafeAreaView style={styles.container}>
      <View style={styles.header}>
        <Text style={styles.welcomeText}>Welcome, {user?.name}</Text>
        <TouchableOpacity onPress={logout}>
          <MaterialIcons name="logout" size={24} color="white" />
        </TouchableOpacity>
      </View>
      
      <ScrollView 
        style={styles.content}
        refreshControl={
          <RefreshControl refreshing={refreshing} onRefresh={onRefresh} />
        }
      >
        {/* Balance Card */}
        <View style={styles.balanceCard}>
          <Text style={styles.balanceLabel}>Total Balance</Text>
          <Text style={styles.balance}>৳{dashboardData?.balance?.toFixed(2) || '0.00'}</Text>
        </View>
        
        {/* Quick Actions */}
        <View style={styles.actionsContainer}>
          <TouchableOpacity style={styles.actionButton} onPress={() => setShowSendMoney(true)}>
            <MaterialIcons name="send" size={24} color="white" />
            <Text style={styles.actionText}>Send Money</Text>
          </TouchableOpacity>
          
          <TouchableOpacity style={styles.actionButton}>
            <MaterialIcons name="money-off" size={24} color="white" />
            <Text style={styles.actionText}>Cash Out</Text>
          </TouchableOpacity>
          
          <TouchableOpacity style={styles.actionButton}>
            <MaterialIcons name="add-circle" size={24} color="white" />
            <Text style={styles.actionText}>Cash In</Text>
          </TouchableOpacity>
          
          <TouchableOpacity style={styles.actionButton}>
            <MaterialIcons name="smartphone" size={24} color="white" />
            <Text style={styles.actionText}>Recharge</Text>
          </TouchableOpacity>
        </View>
        
        {/* Recent Transactions */}
        <View style={styles.section}>
          <View style={styles.sectionHeader}>
            <Text style={styles.sectionTitle}>Recent Transactions</Text>
            <TouchableOpacity onPress={fetchTransactions}>
              <Text style={styles.seeAll}>See All</Text>
            </TouchableOpacity>
          </View>
          
          {dashboardData?.recentTransactions?.length > 0 ? (
            dashboardData.recentTransactions.map(transaction => (
              <View key={transaction._id} style={styles.transactionItem}>
                <View>
                  <Text style={styles.transactionType}>{transaction.type.replace('_', ' ')}</Text>
                  <Text style={styles.transactionDate}>
                    {new Date(transaction.createdAt).toLocaleDateString()}
                  </Text>
                </View>
                <Text style={[
                  styles.transactionAmount,
                  transaction.type === 'send_money' || transaction.type === 'cash_out' 
                    ? styles.negativeAmount 
                    : styles.positiveAmount
                ]}>
                  {transaction.type === 'send_money' || transaction.type === 'cash_out' ? '-' : '+'}
                  ৳{transaction.amount.toFixed(2)}
                </Text>
              </View>
            ))
          ) : (
            <Text style={styles.noData}>No transactions yet</Text>
          )}
        </View>
        
        {/* Savings */}
        <View style={styles.section}>
          <View style={styles.sectionHeader}>
            <Text style={styles.sectionTitle}>Savings</Text>
            <TouchableOpacity onPress={() => setShowSavings(true)}>
              <Text style={styles.seeAll}>Add New</Text>
            </TouchableOpacity>
          </View>
          
          {savings.length > 0 ? (
            savings.map(account => (
              <View key={account._id} style={styles.savingsItem}>
                <View>
                  <Text style={styles.savingsName}>{account.name}</Text>
                  <Text style={styles.savingsType}>{account.type}</Text>
                </View>
                <Text style={styles.savingsBalance}>৳{account.balance.toFixed(2)}</Text>
              </View>
            ))
          ) : (
            <Text style={styles.noData}>No savings accounts</Text>
          )}
        </View>
        
        {/* Quick Links */}
        <View style={styles.quickLinks}>
          <TouchableOpacity style={styles.quickLink} onPress={generateQRCode}>
            <FontAwesome5 name="qrcode" size={20} color="#3498db" />
            <Text style={styles.quickLinkText}>My QR</Text>
          </TouchableOpacity>
          
          <TouchableOpacity style={styles.quickLink} onPress={() => setShowKYC(true)}>
            <FontAwesome5 name="id-card" size={20} color="#3498db" />
            <Text style={styles.quickLinkText}>KYC</Text>
          </TouchableOpacity>
          
          <TouchableOpacity style={styles.quickLink}>
            <FontAwesome5 name="history" size={20} color="#3498db" />
            <Text style={styles.quickLinkText}>History</Text>
          </TouchableOpacity>
          
          <TouchableOpacity style={styles.quickLink}>
            <FontAwesome5 name="cog" size={20} color="#3498db" />
            <Text style={styles.quickLinkText}>Settings</Text>
          </TouchableOpacity>
        </View>
      </ScrollView>
      
      {/* Send Money Modal */}
      <Modal visible={showSendMoney} animationType="slide">
        <View style={styles.modalContainer}>
          <View style={styles.modalHeader}>
            <Text style={styles.modalTitle}>Send Money</Text>
            <TouchableOpacity onPress={() => setShowSendMoney(false)}>
              <MaterialIcons name="close" size={24} color="black" />
            </TouchableOpacity>
          </View>
          
          <TextInput
            style={styles.input}
            placeholder="Recipient Phone Number"
            value={recipientPhone}
            onChangeText={setRecipientPhone}
            keyboardType="phone-pad"
          />
          
          <TextInput
            style={styles.input}
            placeholder="Amount"
            value={amount}
            onChangeText={setAmount}
            keyboardType="decimal-pad"
          />
          
          <TextInput
            style={styles.input}
            placeholder="Description (Optional)"
            value={description}
            onChangeText={setDescription}
          />
          
          <TouchableOpacity 
            style={styles.button} 
            onPress={handleSendMoney}
            disabled={loading}
          >
            {loading ? (
              <ActivityIndicator color="white" />
            ) : (
              <Text style={styles.buttonText}>Send Money</Text>
            )}
          </TouchableOpacity>
        </View>
      </Modal>
      
      {/* KYC Modal */}
      <Modal visible={showKYC} animationType="slide">
        <View style={styles.modalContainer}>
          <View style={styles.modalHeader}>
            <Text style={styles.modalTitle}>KYC Verification</Text>
            <TouchableOpacity onPress={() => setShowKYC(false)}>
              <MaterialIcons name="close" size={24} color="black" />
            </TouchableOpacity>
          </View>
          
          <Text style={styles.modalSubtitle}>Upload your NID/Passport</Text>
          
          <TouchableOpacity style={styles.uploadButton} onPress={pickDocument}>
            <MaterialIcons name="cloud-upload" size={24} color="#3498db" />
            <Text style={styles.uploadText}>Upload Document</Text>
          </TouchableOpacity>
          
          {kycDocuments.length > 0 && (
            <Text style={styles.uploadedText}>
              {kycDocuments.length} document(s) uploaded
            </Text>
          )}
          
          <TouchableOpacity 
            style={styles.button} 
            onPress={submitKYC}
            disabled={loading}
          >
            {loading ? (
              <ActivityIndicator color="white" />
            ) : (
              <Text style={styles.buttonText}>Submit</Text>
            )}
          </TouchableOpacity>
        </View>
      </Modal>
      
      {/* QR Code Modal */}
      <Modal visible={showQR} animationType="slide">
        <View style={styles.modalContainer}>
          <View style={styles.modalHeader}>
            <Text style={styles.modalTitle}>My QR Code</Text>
            <TouchableOpacity onPress={() => setShowQR(false)}>
              <MaterialIcons name="close" size={24} color="black" />
            </TouchableOpacity>
          </View>
          
          {qrCode ? (
            <Image source={{ uri: qrCode }} style={styles.qrCode} />
          ) : (
            <ActivityIndicator size="large" color="#3498db" />
          )}
          
          <Text style={styles.qrInfo}>Scan this QR code to send money</Text>
        </View>
      </Modal>
      
      {/* Savings Modal */}
      <Modal visible={showSavings} animationType="slide">
        <View style={styles.modalContainer}>
          <View style={styles.modalHeader}>
            <Text style={styles.modalTitle}>Create Savings</Text>
            <TouchableOpacity onPress={() => setShowSavings(false)}>
              <MaterialIcons name="close" size={24} color="black" />
            </TouchableOpacity>
          </View>
          
          <View style={styles.savingsTypeContainer}>
            <TouchableOpacity 
              style={[styles.savingsTypeButton, savingsType === 'flexi' && styles.activeSavingsType]} 
              onPress={() => setSavingsType('flexi')}
            >
              <Text style={styles.savingsTypeText}>Flexi</Text>
            </TouchableOpacity>
            <TouchableOpacity 
              style={[styles.savingsTypeButton, savingsType === 'fixed' && styles.activeSavingsType]} 
              onPress={() => setSavingsType('fixed')}
            >
              <Text style={styles.savingsTypeText}>Fixed</Text>
            </TouchableOpacity>
            <TouchableOpacity 
              style={[styles.savingsTypeButton, savingsType === 'goal' && styles.activeSavingsType]} 
              onPress={() => setSavingsType('goal')}
            >
              <Text style={styles.savingsTypeText}>Goal</Text>
            </TouchableOpacity>
          </View>
          
          <TextInput
            style={styles.input}
            placeholder="Savings Name"
            value={savingsName}
            onChangeText={setSavingsName}
          />
          
          <TextInput
            style={styles.input}
            placeholder="Target Amount"
            value={targetAmount}
            onChangeText={setTargetAmount}
            keyboardType="decimal-pad"
          />
          
          <TextInput
            style={styles.input}
            placeholder="Interest Rate (%)"
            value={interestRate}
            onChangeText={setInterestRate}
            keyboardType="decimal-pad"
          />
          
          <TouchableOpacity 
            style={styles.button} 
            onPress={createSavings}
            disabled={loading}
          >
            {loading ? (
              <ActivityIndicator color="white" />
            ) : (
              <Text style={styles.buttonText}>Create Savings</Text>
            )}
          </TouchableOpacity>
        </View>
      </Modal>
    </SafeAreaView>
  );

  return token ? renderDashboard() : renderAuthScreen();
}

// Styles
const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#f5f5f5',
  },
  authContainer: {
    flex: 1,
    justifyContent: 'center',
    padding: 20,
  },
  title: {
    fontSize: 28,
    fontWeight: 'bold',
    textAlign: 'center',
    marginBottom: 30,
    color: '#3498db',
  },
  toggleContainer: {
    flexDirection: 'row',
    marginBottom: 20,
    borderRadius: 8,
    overflow: 'hidden',
    borderWidth: 1,
    borderColor: '#ddd',
  },
  toggleButton: {
    flex: 1,
    paddingVertical: 12,
    alignItems: 'center',
    backgroundColor: '#f9f9f9',
  },
  activeToggle: {
    backgroundColor: '#3498db',
  },
  toggleText: {
    fontWeight: 'bold',
    color: '#333',
  },
  input: {
    height: 50,
    borderColor: '#ddd',
    borderWidth: 1,
    borderRadius: 8,
    paddingHorizontal: 15,
    marginBottom: 15,
    backgroundColor: 'white',
  },
  button: {
    height: 50,
    backgroundColor: '#3498db',
    borderRadius: 8,
    justifyContent: 'center',
    alignItems: 'center',
    marginTop: 10,
  },
  buttonText: {
    color: 'white',
    fontWeight: 'bold',
    fontSize: 16,
  },
  header: {
    backgroundColor: '#3498db',
    padding: 20,
    flexDirection: 'row',
    justifyContent: 'space-between',
    alignItems: 'center',
  },
  welcomeText: {
    color: 'white',
    fontSize: 18,
    fontWeight: 'bold',
  },
  content: {
    flex: 1,
    padding: 20,
  },
  balanceCard: {
    backgroundColor: '#3498db',
    borderRadius: 10,
    padding: 20,
    marginBottom: 20,
    shadowColor: '#000',
    shadowOffset: { width: 0, height: 2 },
    shadowOpacity: 0.1,
    shadowRadius: 5,
    elevation: 3,
  },
  balanceLabel: {
    color: 'white',
    fontSize: 16,
    marginBottom: 5,
  },
  balance: {
    color: 'white',
    fontSize: 28,
    fontWeight: 'bold',
  },
  actionsContainer: {
    flexDirection: 'row',
    flexWrap: 'wrap',
    justifyContent: 'space-between',
    marginBottom: 20,
  },
  actionButton: {
    width: '48%',
    height: 100,
    backgroundColor: '#3498db',
    borderRadius: 10,
    justifyContent: 'center',
    alignItems: 'center',
    marginBottom: 15,
    shadowColor: '#000',
    shadowOffset: { width: 0, height: 2 },
    shadowOpacity: 0.1,
    shadowRadius: 5,
    elevation: 3,
  },
  actionText: {
    color: 'white',
    fontWeight: 'bold',
    marginTop: 5,
  },
  section: {
    marginBottom: 20,
  },
  sectionHeader: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    alignItems: 'center',
    marginBottom: 10,
  },
  sectionTitle: {
    fontSize: 18,
    fontWeight: 'bold',
    color: '#333',
  },
  seeAll: {
    color: '#3498db',
    fontWeight: 'bold',
  },
  transactionItem: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    alignItems: 'center',
    paddingVertical: 12,
    borderBottomWidth: 1,
    borderBottomColor: '#eee',
  },
  transactionType: {
    fontSize: 16,
    fontWeight: 'bold',
    color: '#333',
  },
  transactionDate: {
    fontSize: 12,
    color: '#888',
  },
  transactionAmount: {
    fontSize: 16,
    fontWeight: 'bold',
  },
  positiveAmount: {
    color: '#27ae60',
  },
  negativeAmount: {
    color: '#e74c3c',
  },
  noData: {
    textAlign: 'center',
    color: '#888',
    paddingVertical: 20,
  },
  savingsItem: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    alignItems: 'center',
    paddingVertical: 12,
    borderBottomWidth: 1,
    borderBottomColor: '#eee',
  },
  savingsName: {
    fontSize: 16,
    fontWeight: 'bold',
    color: '#333',
  },
  savingsType: {
    fontSize: 12,
    color: '#888',
  },
  savingsBalance: {
    fontSize: 16,
    fontWeight: 'bold',
    color: '#27ae60',
  },
  quickLinks: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    marginTop: 10,
  },
  quickLink: {
    alignItems: 'center',
    width: '22%',
  },
  quickLinkText: {
    marginTop: 5,
    fontSize: 12,
    color: '#3498db',
  },
  modalContainer: {
    flex: 1,
    padding: 20,
    backgroundColor: 'white',
  },
  modalHeader: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    alignItems: 'center',
    marginBottom: 20,
  },
  modalTitle: {
    fontSize: 20,
    fontWeight: 'bold',
    color: '#333',
  },
  modalSubtitle: {
    fontSize: 16,
    color: '#666',
    marginBottom: 20,
    textAlign: 'center',
  },
  uploadButton: {
    borderWidth: 1,
    borderColor: '#ddd',
    borderStyle: 'dashed',
    borderRadius: 8,
    padding: 20,
    alignItems: 'center',
    marginBottom: 15,
  },
  uploadText: {
    color: '#3498db',
    fontWeight: 'bold',
    marginTop: 5,
  },
  uploadedText: {
    textAlign: 'center',
    color: '#27ae60',
    marginBottom: 15,
  },
  qrCode: {
    width: 200,
    height: 200,
    alignSelf: 'center',
    marginVertical: 20,
  },
  qrInfo: {
    textAlign: 'center',
    color: '#666',
    marginBottom: 20,
  },
  savingsTypeContainer: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    marginBottom: 15,
  },
  savingsTypeButton: {
    flex: 1,
    paddingVertical: 10,
    alignItems: 'center',
    borderWidth: 1,
    borderColor: '#ddd',
    borderRadius: 8,
    marginHorizontal: 5,
  },
  activeSavingsType: {
    backgroundColor: '#3498db',
    borderColor: '#3498db',
  },
  savingsTypeText: {
    fontWeight: 'bold',
    color: '#333',
  },
});