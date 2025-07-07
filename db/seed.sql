--
-- PostgreSQL database dump
--

-- Dumped from database version 14.18 (Homebrew)
-- Dumped by pg_dump version 15.13 (Homebrew)

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- Data for Name: department; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public.department VALUES (1, 'Engineering');
INSERT INTO public.department VALUES (2, 'Finance');


--
-- Data for Name: job; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public.job VALUES (27, 'Test Job 1751817980705567');
INSERT INTO public.job VALUES (30, 'Test Job 1751826007331756');
INSERT INTO public.job VALUES (4, 'E5');
INSERT INTO public.job VALUES (33, 'Test Job 1751827647496807');
INSERT INTO public.job VALUES (36, 'Test Job 1751861322067051');
INSERT INTO public.job VALUES (28, 'Test Job 1751818751847591');
INSERT INTO public.job VALUES (31, 'Test Job 1751826243991643');
INSERT INTO public.job VALUES (25, 'Test Job 1751812016115629');
INSERT INTO public.job VALUES (34, 'Test Job 1751858205192343');
INSERT INTO public.job VALUES (37, 'Test Job 1751861752130953');
INSERT INTO public.job VALUES (29, 'Test Job 1751819439896843');
INSERT INTO public.job VALUES (26, 'Test Job 1751812998808007');
INSERT INTO public.job VALUES (32, 'Test Job 1751827276806236');
INSERT INTO public.job VALUES (10, 'Engineer');
INSERT INTO public.job VALUES (35, 'Test Job 1751858403269460');
INSERT INTO public.job VALUES (20, 'Manager');


--
-- Data for Name: person; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public.person VALUES (1, NULL, NULL, 1, 'Sabryna', 'Peers', '2014-02-01');
INSERT INTO public.person VALUES (1002, 20, 1, NULL, 'Manager', 'Boss', '2020-01-01');
INSERT INTO public.person VALUES (1001, 10, 1, 1002, 'Test', 'Person', '2023-01-01');
INSERT INTO public.person VALUES (1003, 10, 2, 1002, 'Second', 'Person2', '2023-02-01');
INSERT INTO public.person VALUES (10, 4, NULL, 8, 'Leon', 'JayLee', '2022-02-17');
INSERT INTO public.person VALUES (11, 4, NULL, 8, 'Kaylie', 'Elyse', '2021-01-18');
INSERT INTO public.person VALUES (12, 4, NULL, 8, 'Yancey', 'Trenton', '2022-03-02');
INSERT INTO public.person VALUES (4, 4, NULL, 2, 'Marcia', 'Stuart', '2020-01-11');
INSERT INTO public.person VALUES (8, NULL, NULL, 1, 'Sterling', 'Haley', '2019-11-02');
INSERT INTO public.person VALUES (9, NULL, NULL, 8, 'Melissa', 'Garland', '2017-08-05');
INSERT INTO public.person VALUES (2, NULL, NULL, 1, 'Tayler', 'Shantee', '2018-04-07');
INSERT INTO public.person VALUES (3, NULL, NULL, 1, 'Madonna', 'Axl', '2018-03-08');
INSERT INTO public.person VALUES (5, NULL, NULL, 2, 'Cliff', 'Rosalind', '2021-02-15');
INSERT INTO public.person VALUES (6, NULL, NULL, 3, 'Lake', 'Philippa', '2022-05-21');
INSERT INTO public.person VALUES (7, NULL, NULL, 3, 'Wynne', 'Walker', '2021-12-31');


--
-- Data for Name: person_info; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public.person_info VALUES (2, 2, 2, 2, 'Test', 'User', '2020-01-01', 'M1', 'HR', NULL);
INSERT INTO public.person_info VALUES (3, 2, 2, 2, 'Test', 'User', '2020-01-01', 'M1', 'HR', 'Manager Name');
INSERT INTO public.person_info VALUES (100, 10, 20, 30, 'John', 'Doe', '2023-01-01', 'Engineer', 'Engineering', 'Alice Smith');
INSERT INTO public.person_info VALUES (1, 10, 20, 30, 'John', 'Doe', '2023-01-01', 'Engineer', 'Engineering', 'Alice Smith');


--
-- Data for Name: user; Type: TABLE DATA; Schema: public; Owner: postgres
--



--
-- Data for Name: users; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO public.users VALUES (1, 'admin', 'password');
INSERT INTO public.users VALUES (2, 'dupeuser', '$2a$12$E9vlhCo0.chHc9iGmQ7J5OgTxWLucLAYDQtQ4VipHtnXnij4dkfKm');
INSERT INTO public.users VALUES (3, 'loginuser', '$2a$12$qvMaZ4CVnzNoXejUZc3vR.0y1yOXbg63BLGNutjhJQu5ifpwNcCQq');
INSERT INTO public.users VALUES (4, 'wrongpass', '$2a$12$D45jzQW2dn0XuuO/JZVBiuZyH.we8IHABuEEQPgNi6nCuWbP8YA.C');
INSERT INTO public.users VALUES (5, 'testuser', '$2a$12$dAFQWAuRryMbPlaS0eep3uYFAE8BYtzXS4D7cOF6V5vsT25ls3UZK');
INSERT INTO public.users VALUES (6, 'testuser_1751804314063908', '$2a$12$v.KgxJMU5iaOXbCby2RDIey36Su0yPnIFh1RszBxZGd4UXy.H.fvG');
INSERT INTO public.users VALUES (7, 'testuser_1751804621786187', '$2a$12$1XN/DDI7x9/ofCF8eJUIROGlDt9fcS4XIuZih9uGMLAYiWZIJUMRi');
INSERT INTO public.users VALUES (8, 'testuser_1751807166715946', '$2a$12$RPGEF.WgW5drxqaSCZ7zae3xpLKuzD5NpwIgXCtFZEgtTc68gehLq');
INSERT INTO public.users VALUES (9, 'dupeuser_1751807171070293', '$2a$12$xSkmN3jQJgGr2teULm13C.lunIZhunWuI57UDz.abIyih9EYKSwuS');
INSERT INTO public.users VALUES (10, 'loginuser_1751807175635370', '$2a$12$/uzkxnZe6XbdRW5PZEWmv.HIv59sxQ9GX1.aW80o/iUXoRSRamUQ6');
INSERT INTO public.users VALUES (11, 'wrongpass_1751807182329164', '$2a$12$h7PuoRe.29.EuR/47b8cNushOQpXAX5U6L/HG9LdJ/U0KsBvbGdLO');
INSERT INTO public.users VALUES (12, 'testuser_1751808495285913', '$2a$12$6FyFbzUHNweRy/fZ.yDY3eMtZKvDmDW3wOKwQb2Ouh5E.dIolLUrq');
INSERT INTO public.users VALUES (13, 'dupeuser_1751808499654169', '$2a$12$4uQzNtdZ6IR7Q2xuWVoG6O6Y5onAbvPYEjLZ4UMzni3oCy/QOwnR.');
INSERT INTO public.users VALUES (14, 'loginuser_1751808504199530', '$2a$12$3xphNFmMeCju0A8PcRMn6eSVCUaC0ckb5ztDkLn6e2toWXbbebO9u');
INSERT INTO public.users VALUES (15, 'wrongpass_1751808510797853', '$2a$12$s6aP7oANCgZDuJGLOG805epUO4QRb9WbsDao8NursKAtPd937JeWG');
INSERT INTO public.users VALUES (16, 'testuser_1751810541147965', '$2a$12$1QcRFdC5k52hE28Hg/3g/epkrJAsrwfocH5VuehzMwHb51UModhEa');
INSERT INTO public.users VALUES (17, 'dupeuser_1751810545494405', '$2a$12$ZYRCQC51/oleDLJySd.cN.ld1PAOHcQZHHtKz70I2v2m9CvSqRn2.');
INSERT INTO public.users VALUES (18, 'loginuser_1751810550059228', '$2a$12$072nBb8CYjXt4B4ESO5fueWWvAFxL7ZR83LUf7SKyiqHW5KAWYmAW');
INSERT INTO public.users VALUES (19, 'wrongpass_1751810556686829', '$2a$12$tlA92SyWiE8/zReROQyT5e8uSo0Za2fy5AwQcgyof3LwVvPCsAvf2');
INSERT INTO public.users VALUES (20, 'testuser_1751811190568433', '$2a$12$NoiaS.UZqh8q9AdYDe3R6.gCbRu2S45hAYaH/EtMvz49hSwpiUG6K');
INSERT INTO public.users VALUES (21, 'dupeuser_1751811194920867', '$2a$12$SUNgExwjaExs8jSxIx8qDe4AFtI.Ynnbyj42tvCVdXFJlK4A7Pev.');
INSERT INTO public.users VALUES (22, 'loginuser_1751811199473310', '$2a$12$iRPnQ4YrPY/SmEt4tUmGW.dWOVGSLHRgUV0k9qbqsNkPAGkYWdaWW');
INSERT INTO public.users VALUES (23, 'wrongpass_1751811206109995', '$2a$12$Ueq05p8Uac9GxBsJ.qWqWuMRaYUmwy7pMYl2qifhKvTO0VOTS0nKS');
INSERT INTO public.users VALUES (24, 'testuser_1751811993775365', '$2a$12$v2dK7bkGm/qA/jLhVBRw0esQ.SRKdgdSo9dkJWPPzqGwHLTahFYHW');
INSERT INTO public.users VALUES (25, 'dupeuser_1751811998196744', '$2a$12$kRZAcjwWTLH4mABpHe6Mwud56q/dBSj07GWU1NsKQf3/BZjZYfqfu');
INSERT INTO public.users VALUES (26, 'loginuser_1751812002762740', '$2a$12$hutVXTJpwfy2/v7K55p3qewqN74Ldgb2OTxHa1fj0MZhuPtsCpoB2');
INSERT INTO public.users VALUES (27, 'wrongpass_1751812009446096', '$2a$12$JcBErnZI0Gp7QZzESMOxzO8U/RkE7WgYybgKikqzVtF/cGVNJ.zoW');
INSERT INTO public.users VALUES (28, 'testuser_1751812976648295', '$2a$12$4Bntg22cB9QaRbGIiAsCy.CbTlIpDwjx3.Km.I2.RXef8o1TXtHI6');
INSERT INTO public.users VALUES (29, 'dupeuser_1751812981007003', '$2a$12$df106kyy5rW.SBUDGqh7i.23d1k1.LEwRW2t1B3gD1T0L9jOPTSbG');
INSERT INTO public.users VALUES (30, 'loginuser_1751812985572087', '$2a$12$XCn4Gn65QKubz98AtF..7eYSvOBs8MckYQD2.0XWGO1KnT1O7xx4.');
INSERT INTO public.users VALUES (31, 'wrongpass_1751812992186923', '$2a$12$eLKSHZaPRSXT39PjP2O4j.Z9ssLdMgXkSHpOaNNHqfEOdDuhgTS9S');
INSERT INTO public.users VALUES (32, 'testuser_1751817958617342', '$2a$12$XUtSBEQhf8T9y7XNaMyMd.gNL7tVAhJdPhb5m2DuIedoOOUYtJz7K');
INSERT INTO public.users VALUES (33, 'dupeuser_1751817962967199', '$2a$12$9GV4kM.rZrAPP3j3lceDfudqVs8RRU3la.29Q1zM9PE4It3rlTFeS');
INSERT INTO public.users VALUES (34, 'loginuser_1751817967524156', '$2a$12$ZrziASEVpXXLp0mQG24JZeZdaphDyoGArA7ZwnCE5MvYdVYX/ZvW2');
INSERT INTO public.users VALUES (35, 'wrongpass_1751817974106515', '$2a$12$/AjFNvQGFS2vByP83oNpIOMgR2AGCIFd9N8BeZFLaz08ytLlKLyti');
INSERT INTO public.users VALUES (36, 'testuser_1751818729759697', '$2a$12$IqEedEXlFySosx4Eqo62t.rmyc4TTDyAXpQbv1BXqRyNwpi6GzXkW');
INSERT INTO public.users VALUES (37, 'dupeuser_1751818734091673', '$2a$12$5C3skQMHE.ZhL0gL51bXheyUCsbBVlRTZLVFpKcsFRe/pKiMGBoAK');
INSERT INTO public.users VALUES (38, 'loginuser_1751818738638179', '$2a$12$HvQcnLFHvaq2SZrzWie5oOmot6xPYMU1cv/yW4/F.JIjgv97PoJXW');
INSERT INTO public.users VALUES (39, 'wrongpass_1751818745236348', '$2a$12$0cPTMtwJGFgNgbFLOcckmuZYuKMUwusM08k.PYXnllq8wemSp63Cy');
INSERT INTO public.users VALUES (40, 'testuser_1751819417749838', '$2a$12$4vAUn1ExCScwdUn3YLo2y.DsFk73dsODUSGkhlu9jO8kv/Od0H09W');
INSERT INTO public.users VALUES (41, 'dupeuser_1751819422077959', '$2a$12$QNgx2ga9H/7AJYzCG3f3t.eEWgknPNlLOKWPZq6koensR4CAVv.Pq');
INSERT INTO public.users VALUES (42, 'loginuser_1751819426626859', '$2a$12$d5Rw5llPomXE.cTnXNW1cOwW4DsTvMYSddi1HbXA3jTsJpLE3W43e');
INSERT INTO public.users VALUES (43, 'wrongpass_1751819433228338', '$2a$12$sOXysKTEGKdQmxq2WhhBYuXeAIpZ5L82YLbfKTskCcGJ7hC8QOooy');
INSERT INTO public.users VALUES (44, 'testuser_1751825985184717', '$2a$12$0wTOCqzG0r5N3XwB1x3y7.a8YbJQP0ZG/lPJSbhx8c0jfPN7kstNq');
INSERT INTO public.users VALUES (45, 'dupeuser_1751825989536035', '$2a$12$Nf4Y2gPPLOiNbtlI1ev2yOuNlJffIJPVlxT3IuChKlu.H.77pH0fO');
INSERT INTO public.users VALUES (46, 'loginuser_1751825994096110', '$2a$12$gvG5NMjZQ/oPIp/uTfwCfeql.6qQoJueDdAwmq.7yX4kwpJI0eZ4G');
INSERT INTO public.users VALUES (47, 'wrongpass_1751826000709690', '$2a$12$DBqfwd6SdoQe6rnJqlgY/exF8/yP8TGF7138o8Lh38kwbCc5v5DIi');
INSERT INTO public.users VALUES (48, 'testuser_1751826242502989', '$2a$12$LvqNexgXABYXyZcAQBbUo.RUZi2UKhlrm3VTPlUb0qiILx0oR4986');
INSERT INTO public.users VALUES (49, 'dupeuser_1751826242724570', '$2a$12$z9v3ml3p1MuY47B0LZS5OuAPbwZYsB8MZS/p6aI1jQQdfAhuHyPKu');
INSERT INTO public.users VALUES (50, 'loginuser_1751826243148579', '$2a$12$I3AppivulS/41HQ3hLQumudDoWff1G45kfzVhVelK2Az3vlY4c0V.');
INSERT INTO public.users VALUES (51, 'wrongpass_1751826243567956', '$2a$12$MXSBuKtBqyDaNZMUh7H6/.vgVJbZO3qYuC6agI8RP5kusKrpzbkBm');
INSERT INTO public.users VALUES (52, 'testuser_1751827275286131', '$2a$12$5hSV1KjjI9Jc9gMrPhhsh.PEG6nXdSm4TtZOUNwsqLeCOuSIDPLvm');
INSERT INTO public.users VALUES (53, 'dupeuser_1751827275511664', '$2a$12$OlBCG.J/89Rvo5SFKhwxeuGu5bAQz8ohdb37NTN/vb3by7dInGbEG');
INSERT INTO public.users VALUES (54, 'loginuser_1751827275945725', '$2a$12$CzlMA3mIWgyfFtz5br7hVuYK1jjAM9HVm0Q52dG43J65aN/6V6oNK');
INSERT INTO public.users VALUES (55, 'wrongpass_1751827276375049', '$2a$12$0FOg8CWT6/Au77XJjhwDMehiNy5S049AOCMsyKvCp8Mtdp22dqfRW');
INSERT INTO public.users VALUES (56, 'testuser_1751827645817054', '$2a$12$H2oBaOuqvrQC152Lfy.zWeNt5dPdJLwtfEizwC3opUZbTrviV4CJS');
INSERT INTO public.users VALUES (57, 'dupeuser_1751827646074038', '$2a$12$.ZD1IRQ11U/h22OL82SQSeAMLay8enKuPmo7dEdsQsnf4G2wMdlXm');
INSERT INTO public.users VALUES (58, 'loginuser_1751827646583069', '$2a$12$oTPgAGdFrpdLw1mbnddqdO5KlhdhS95v2DJht8EDLPsc4qjxJKzFO');
INSERT INTO public.users VALUES (59, 'wrongpass_1751827647050096', '$2a$12$3rJrDmz9IbvVx6I9KxpySO4frB/Zxsc9lDp5yCYuWk9l6tweNNQi.');
INSERT INTO public.users VALUES (60, 'testuser_1751858203368142', '$2a$12$akjlKyrqsMY2lBPppYyRAuOCpwOH4X3xUpzfOB5lEjugqicCZT4Uq');
INSERT INTO public.users VALUES (61, 'dupeuser_1751858203772994', '$2a$12$pealncjlkCGJL2ZXXsk0fOcUGc3TuAb2f2oKGdjtpgQXpIQVrxPkK');
INSERT INTO public.users VALUES (62, 'loginuser_1751858204281385', '$2a$12$.IVfxMefDeHbQWD5mnyRouqDcem512zYppB7mU83CjwTxNn2BjQGC');
INSERT INTO public.users VALUES (63, 'wrongpass_1751858204736258', '$2a$12$RVGcnpnpfm4FJSccv/k7GuBvwDATNDE/ewee/kvtT4Sb7AYRHZs5W');
INSERT INTO public.users VALUES (64, 'testuser_1751858380784787', '$2a$12$6zia73.nUoEBlHkOu0VBHuCkNyJo6GpYsqjNyXUIZ8epC8nwkxrQS');
INSERT INTO public.users VALUES (65, 'dupeuser_1751858385158574', '$2a$12$K0h3/cnX.TON.qV3PP5o0.0l/n1KuN8uU7m7L/8jCS/BzKmfDea8y');
INSERT INTO public.users VALUES (66, 'loginuser_1751858389767408', '$2a$12$FKFnBL7M2mxxuJTmdG292uWES6cP.OD7vvC5yVaDm1jji.DJZqys6');
INSERT INTO public.users VALUES (67, 'wrongpass_1751858396548284', '$2a$12$JlKPHkSuG5HuBFnqoc9PduvrvP1fAHjBJK3oUL.eex4cQtBPjFkHa');
INSERT INTO public.users VALUES (68, 'testuser_1751861299649329', '$2a$12$Tq2nAa8oKDtLS7KTqvnX3eoWY.APlhtZyo4w/KOJHiSDZbJmTnDUy');
INSERT INTO public.users VALUES (69, 'dupeuser_1751861304082653', '$2a$12$VnQpPQiiXB2UknaUbCVrwO7ZWwcDRiyYOxtj3PQpREKHmK83QLqGC');
INSERT INTO public.users VALUES (70, 'loginuser_1751861308694865', '$2a$12$gJpVW.lzuw3qzWVnTDT4G.Wh4s2q/iBEKex/sh/64AhBsfaXa02mu');
INSERT INTO public.users VALUES (71, 'wrongpass_1751861315372881', '$2a$12$aekY5QjtlYGoCiVnRhRdZuVfYGUHrDEISW3N.AEUCHVBl.8I8Rc0u');
INSERT INTO public.users VALUES (72, 'testuser_1751861747770452', '$2a$12$tjE0wCHXpKyR/R3LgzEEXeRE.q9dglDdb9TinDgYOoq/1HiqX0p7a');
INSERT INTO public.users VALUES (73, 'dupeuser_1751861748410109', '$2a$12$C4ASZkcDrTD50X43WeBCou0R06KQUzmh7TNQ3xuqPw6CF.Gxo6i12');
INSERT INTO public.users VALUES (74, 'loginuser_1751861749417388', '$2a$12$rOIGiOMtNG95VOm6/OeF7OuIDO14LqUK0veiP6XKO5dWpqqhOMkXC');
INSERT INTO public.users VALUES (75, 'wrongpass_1751861750972311', '$2a$12$Lk5oIS4h/HCvLzopXxjDCeQvzM.Bw385KtQ2hPv26uXVBvjtrOhdK');


--
-- Name: department_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.department_id_seq', 60, true);


--
-- Name: job_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.job_id_seq', 37, true);


--
-- Name: person_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.person_id_seq', 24, true);


--
-- Name: person_info_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.person_info_id_seq', 1, false);


--
-- Name: user_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.user_id_seq', 1, false);


--
-- Name: users_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.users_id_seq', 75, true);


--
-- PostgreSQL database dump complete
--

