#include<iostream>
#include<vector>
#include<cstdio>
using namespace std;
#define For(i,l,r) for(int i=l;i<=r;i++)
#define P 998244353
#define G 3
#define GI 332748118
#define I 911660635
int pow(int a,int b){int ans=1;while(b)(b&1)&&(ans=1ll*ans*a%P),a=1ll*a*a%P,b>>=1;return ans;}
namespace Sqrt
{
	int mul;
	struct Complex
	{
		int a,b;Complex(int x,int y){a=x,b=y;}
		friend Complex operator*(Complex a,Complex b){return Complex((1ll*a.a*b.a+1ll*a.b*b.b%P*mul)%P,(1ll*a.a*b.b+1ll*a.b*b.a)%P);}
	};
	Complex Pow(Complex a,int b){Complex ans(1,0);while(b)(b&1)&&(ans=ans*a,0),a=a*a,b>>=1;return ans;}
	int sqrt(int x){int a=rand()%P;while(pow((1ll*a*a-x+P)%P,P-1>>1)==1)a=rand()%P;mul=(1ll*a*a-x+P)%P;Complex y={a,1};int ans=Pow(y,P+1>>1).a;return min(ans,P-ans);}
}
using Sqrt::sqrt;
struct poly
{
	int N,K;vector<int>a;poly(){N=K=0;a.clear();}poly(int x){N=K=1;a.resize(1);a[0]=x;}poly(int M,vector<int>x){N=M;K=1;while(K<N)K<<=1;a=x;a.resize(K);}
	void NTT(int opt)
	{
		vector<int>r(K);For(i,0,K-1)r[i]=(r[i>>1]>>1)|((i&1)*(K>>1));For(i,0,K-1)if(i<r[i])swap(a[i],a[r[i]]);
		for(int i=2;i<=K;i<<=1){int W=pow(opt==1?G:GI,(P-1)/i);for(int j=0;j<K;j+=i){int w=1;For(k,0,(i>>1)-1){int x=a[j+k],y=1ll*w*a[j+k+(i>>1)]%P;a[j+k]=(x+y)%P;a[j+k+(i>>1)]=(x-y+P)%P;w=1ll*w*W%P;}}}
		if(opt==-1){int x=pow(K,P-2);For(i,0,K-1)a[i]=1ll*a[i]*x%P;}
	}
	poly Changelength(int x){poly ans;ans.N=x;ans.K=1;while(ans.K<x)ans.K<<=1;ans.a=a;ans.a.resize(ans.K);For(i,min(N,x),ans.K-1)ans[i]=0;return ans;}
	int&operator[](int x){return a[x];}
	friend poly operator+(poly a,poly b){a.N=max(a.N,b.N);a.K=max(a.K,b.K);a.a.resize(a.K);b.a.resize(a.K);For(i,0,a.N-1)a[i]=(a[i]+b[i])%P;return a;}
	friend poly operator+(poly a,int b){a[0]=(a[0]+b)%P;return a;}
	friend poly operator-(poly a){For(i,0,a.N-1)a[i]=(P-a[i])%P;return a;}
	friend poly operator-(poly a,poly b){return a+(-b);}
	friend poly operator-(poly a,int b){return a+(P-b);}
	friend poly operator*(poly a,poly b){int L=a.N+b.N-1;a=a.Changelength(L);b=b.Changelength(L);a.NTT(1);b.NTT(1);For(i,0,a.K-1)a[i]=1ll*a[i]*b[i]%P;a.NTT(-1);return a;}
	friend poly operator*(poly a,int b){For(i,0,a.N-1)a[i]=1ll*a[i]*b%P;return a;}
	friend pair<poly,poly>operator/(poly a,poly b){poly c=(a.Reverse()*b.Reverse().Changelength(a.N-b.N+1).Inv()).Changelength(a.N-b.N+1).Reverse(),d=(a-b*c).Changelength(b.N-1);return {c,d};}
	friend poly operator/(poly a,int b){return a*pow(b,P-2);}
	poly Reverse(){poly ans;ans.N=N;ans.K=K;ans.a.resize(K);For(i,0,N-1)ans[i]=a[N-1-i];return ans;}
	poly Inv()
	{
		if(N==1)return poly(pow(a[0],P-2));
		poly tmp=Changelength(N+1>>1).Inv();return (tmp*2-tmp*tmp*(*this)).Changelength(N);
	}
	poly Sqrt()
	{
		if(N==1)return sqrt(a[0]);
		poly tmp=Changelength(N+1>>1).Sqrt();return ((tmp+(*this)*tmp.Changelength(N).Inv())/2).Changelength(N);
	}
	poly Delta(){poly ans;ans.N=N;ans.K=K;ans.a.resize(ans.K);For(i,0,N-2)ans[i]=1ll*(i+1)*a[i+1]%P;return ans;}
	poly Sum(){poly ans;ans.N=N;ans.K=K;ans.a.resize(ans.K);For(i,1,N)ans[i]=1ll*pow(i,P-2)*a[i-1]%P;return ans;}
	poly Ln(){return (Delta()*Inv()).Changelength(N).Sum().Changelength(N);}
	poly Exp()
	{
		if(N==1)return poly(1);
		poly tmp=Changelength(N+1>>1).Exp();return (tmp*(poly(1)-tmp.Changelength(N).Ln()+*this)).Changelength(N);
	}
	poly Pow(int x){return (Ln()*x).Exp();}
	poly Cos(){return (((*this)*I).Exp()+((*this)*(P-I)).Exp())/2;}
	poly Sin(){return (((*this)*I).Exp()-((*this)*(P-I)).Exp())/2/I;}
	void Print(){For(i,0,N-1)cout<<a[i]<<" \n"[i==N-1];}
};