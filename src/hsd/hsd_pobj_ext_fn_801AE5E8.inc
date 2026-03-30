    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r26, 0x18(r1)
    mr. r30, r3
    beq @L_801AEBC8
    lis r3, -32698
    addi r3, r3, 22196
    bl fn_801AA4CC
    mr. r26, r3
    bne @L_801AE624
    addi r3, r2, -22864
    li r4, 0x3c5
    addi r5, r2, -22816
    bl fn_80196E10
@L_801AE624:
    mr r3, r26
    li r4, 0x0
    li r5, 0x1c
    bl memset
    lwz r29, 0x0(r30)
    mr r31, r26
    cmplwi r29, 0x0
    beq @L_801AE948
    lis r3, -32698
    addi r3, r3, 22196
    bl fn_801AA4CC
    lis r5, -32698
    lis r4, -32729
    addi r0, r5, 22196
    mr r26, r3
    addi r4, r4, 21116
    mr r3, r0
    bl fn_801AEBE0
    cmplwi r26, 0x0
    bne @L_801AE684
    addi r3, r2, -22864
    li r4, 0x3c5
    addi r5, r2, -22816
    bl fn_80196E10
@L_801AE684:
    mr r3, r26
    li r4, 0x0
    li r5, 0x1c
    bl memset
    lwz r27, 0x0(r29)
    cmplwi r27, 0x0
    beq @L_801AE7AC
    bl fn_801AE4B0
    mr r28, r3
    lwz r3, 0x0(r27)
    bl fn_801AE5E8
    stw r3, 0x0(r28)
    lis r0, 0x2000
    lwz r3, 0x4(r27)
    stw r3, 0x4(r28)
    lwz r3, 0x4(r28)
    rlwinm r3, r3, 0, 1, 3
    cmpw r3, r0
    beq @L_801AE710
    bge @L_801AE6F0
    lis r0, 0x1000
    cmpw r3, r0
    beq @L_801AE7B0
    bge @L_801AE794
    cmpwi r3, 0x0
    beq @L_801AE74C
    b @L_801AE794
@L_801AE6F0:
    lis r0, 0x4000
    cmpw r3, r0
    beq @L_801AE778
    bge @L_801AE794
    lis r0, 0x3000
    cmpw r3, r0
    beq @L_801AE75C
    b @L_801AE794
@L_801AE710:
    lwz r0, 0x4(r28)
    rlwinm r0, r0, 0, 4, 31
    cmpwi r0, 0x7
    bge @L_801AE740
    cmpwi r0, 0x1
    bge @L_801AE72C
    b @L_801AE740
@L_801AE72C:
    lfs f1, -22820(r2)
    lfs f0, 0x8(r27)
    fmuls f0, f1, f0
    stfs f0, 0x8(r28)
    b @L_801AE7B0
@L_801AE740:
    lfs f0, 0x8(r27)
    stfs f0, 0x8(r28)
    b @L_801AE7B0
@L_801AE74C:
    lwz r4, 0x8(r27)
    addi r3, r28, 0x8
    bl fn_801ADE50
    b @L_801AE7B0
@L_801AE75C:
    lwz r4, 0x8(r27)
    addi r3, r28, 0x8
    bl fn_801ADD48
    lwz r0, 0x4(r28)
    rlwinm r0, r0, 0, 4, 0
    stw r0, 0x4(r28)
    b @L_801AE7B0
@L_801AE778:
    lwz r3, 0x8(r27)
    lfs f0, 0x0(r3)
    stfs f0, 0x8(r28)
    lwz r3, 0x8(r27)
    lfs f0, 0x4(r3)
    stfs f0, 0xc(r28)
    b @L_801AE7B0
@L_801AE794:
    lis r4, -32729
    addi r3, r2, -22864
    addi r5, r4, 21128
    li r4, 0x37d
    bl fn_80196D78
    b @L_801AE7B0
@L_801AE7AC:
    li r28, 0x0
@L_801AE7B0:
    stw r28, 0x0(r26)
    lis r0, 0x2000
    lwz r3, 0x4(r29)
    stw r3, 0x4(r26)
    lwz r3, 0x4(r26)
    rlwinm r3, r3, 0, 1, 3
    cmpw r3, r0
    beq @L_801AE810
    bge @L_801AE7F0
    lis r0, 0x1000
    cmpw r3, r0
    beq @L_801AE94C
    bge @L_801AE930
    cmpwi r3, 0x0
    beq @L_801AE84C
    b @L_801AE930
@L_801AE7F0:
    lis r0, 0x4000
    cmpw r3, r0
    beq @L_801AE914
    bge @L_801AE930
    lis r0, 0x3000
    cmpw r3, r0
    beq @L_801AE8A8
    b @L_801AE930
@L_801AE810:
    lwz r0, 0x4(r26)
    rlwinm r0, r0, 0, 4, 31
    cmpwi r0, 0x7
    bge @L_801AE840
    cmpwi r0, 0x1
    bge @L_801AE82C
    b @L_801AE840
@L_801AE82C:
    lfs f1, -22820(r2)
    lfs f0, 0x8(r29)
    fmuls f0, f1, f0
    stfs f0, 0x8(r26)
    b @L_801AE94C
@L_801AE840:
    lfs f0, 0x8(r29)
    stfs f0, 0x8(r26)
    b @L_801AE94C
@L_801AE84C:
    addi r27, r26, 0x8
    lwz r28, 0x8(r29)
    li r4, 0x0
    li r5, 0x10
    mr r3, r27
    bl memset
    cmplwi r28, 0x0
    beq @L_801AE94C
    lwz r0, 0x0(r28)
    cmplwi r0, 0x0
    beq @L_801AE884
    lwz r0, 0x0(r28)
    stw r0, 0x0(r27)
    b @L_801AE890
@L_801AE884:
    lis r3, -32741
    addi r0, r3, -8192
    stw r0, 0x0(r27)
@L_801AE890:
    lwz r3, 0x4(r28)
    bl fn_801ADF54
    stw r3, 0x4(r27)
    li r0, -0x1
    stw r0, 0x8(r27)
    b @L_801AE94C
@L_801AE8A8:
    addi r27, r26, 0x8
    lwz r28, 0x8(r29)
    li r4, 0x0
    li r5, 0x10
    mr r3, r27
    bl memset
    cmplwi r28, 0x0
    beq @L_801AE904
    lwz r0, 0x0(r28)
    cmplwi r0, 0x0
    beq @L_801AE8E0
    lwz r0, 0x0(r28)
    stw r0, 0x0(r27)
    b @L_801AE8E8
@L_801AE8E0:
    li r0, 0x0
    stw r0, 0x0(r27)
@L_801AE8E8:
    lwz r3, 0x4(r28)
    bl fn_801ADF54
    stw r3, 0x4(r27)
    li r4, -0x1
    li r0, 0x1
    stw r4, 0x8(r27)
    stb r0, 0xc(r27)
@L_801AE904:
    lwz r0, 0x4(r26)
    rlwinm r0, r0, 0, 4, 0
    stw r0, 0x4(r26)
    b @L_801AE94C
@L_801AE914:
    lwz r3, 0x8(r29)
    lfs f0, 0x0(r3)
    stfs f0, 0x8(r26)
    lwz r3, 0x8(r29)
    lfs f0, 0x4(r3)
    stfs f0, 0xc(r26)
    b @L_801AE94C
@L_801AE930:
    lis r4, -32729
    addi r3, r2, -22864
    addi r5, r4, 21128
    li r4, 0x37d
    bl fn_80196D78
    b @L_801AE94C
@L_801AE948:
    li r26, 0x0
@L_801AE94C:
    stw r26, 0x0(r31)
    lis r0, 0x2000
    lwz r3, 0x4(r30)
    stw r3, 0x4(r31)
    lwz r3, 0x4(r31)
    rlwinm r3, r3, 0, 1, 3
    cmpw r3, r0
    beq @L_801AE9AC
    bge @L_801AE98C
    lis r0, 0x1000
    cmpw r3, r0
    beq @L_801AEBC0
    bge @L_801AEBAC
    cmpwi r3, 0x0
    beq @L_801AE9E8
    b @L_801AEBAC
@L_801AE98C:
    lis r0, 0x4000
    cmpw r3, r0
    beq @L_801AEB90
    bge @L_801AEBAC
    lis r0, 0x3000
    cmpw r3, r0
    beq @L_801AEAB4
    b @L_801AEBAC
@L_801AE9AC:
    lwz r0, 0x4(r31)
    rlwinm r0, r0, 0, 4, 31
    cmpwi r0, 0x7
    bge @L_801AE9DC
    cmpwi r0, 0x1
    bge @L_801AE9C8
    b @L_801AE9DC
@L_801AE9C8:
    lfs f1, -22820(r2)
    lfs f0, 0x8(r30)
    fmuls f0, f1, f0
    stfs f0, 0x8(r31)
    b @L_801AEBC0
@L_801AE9DC:
    lfs f0, 0x8(r30)
    stfs f0, 0x8(r31)
    b @L_801AEBC0
@L_801AE9E8:
    lwz r26, 0x8(r30)
    addi r3, r31, 0x8
    li r4, 0x0
    li r5, 0x10
    bl memset
    cmplwi r26, 0x0
    beq @L_801AEBC0
    lwz r0, 0x0(r26)
    cmplwi r0, 0x0
    beq @L_801AEA1C
    lwz r0, 0x0(r26)
    stw r0, 0x8(r31)
    b @L_801AEA28
@L_801AEA1C:
    lis r3, -32741
    addi r0, r3, -8192
    stw r0, 0x8(r31)
@L_801AEA28:
    lwz r27, 0x4(r26)
    li r0, 0x0
    addi r26, r1, 0xc
    cmplwi r27, 0x0
    stw r0, 0xc(r1)
    bne @L_801AEA94
    li r0, 0x0
    b @L_801AEAA4
@L_801AEA48:
    lis r3, -32698
    addi r3, r3, 22152
    bl fn_801AA4CC
    mr. r28, r3
    bne @L_801AEA6C
    addi r3, r2, -22864
    li r4, 0x485
    addi r5, r2, -22856
    bl fn_80196E10
@L_801AEA6C:
    mr r3, r28
    li r4, 0x0
    li r5, 0xc
    bl memset
    stw r28, 0x0(r26)
    lwz r0, 0x0(r27)
    addi r27, r27, 0x8
    lwz r3, 0x0(r26)
    stw r0, 0x4(r3)
    lwz r26, 0x0(r26)
@L_801AEA94:
    lwz r0, 0x4(r27)
    cmplwi r0, 0x0
    bne @L_801AEA48
    lwz r0, 0xc(r1)
@L_801AEAA4:
    stw r0, 0xc(r31)
    li r0, -0x1
    stw r0, 0x10(r31)
    b @L_801AEBC0
@L_801AEAB4:
    lwz r26, 0x8(r30)
    addi r3, r31, 0x8
    li r4, 0x0
    li r5, 0x10
    bl memset
    cmplwi r26, 0x0
    beq @L_801AEB80
    lwz r0, 0x0(r26)
    cmplwi r0, 0x0
    beq @L_801AEAE8
    lwz r0, 0x0(r26)
    stw r0, 0x8(r31)
    b @L_801AEAF0
@L_801AEAE8:
    li r0, 0x0
    stw r0, 0x8(r31)
@L_801AEAF0:
    lwz r27, 0x4(r26)
    li r0, 0x0
    addi r26, r1, 0x8
    cmplwi r27, 0x0
    stw r0, 0x8(r1)
    bne @L_801AEB5C
    li r0, 0x0
    b @L_801AEB6C
@L_801AEB10:
    lis r3, -32698
    addi r3, r3, 22152
    bl fn_801AA4CC
    mr. r28, r3
    bne @L_801AEB34
    addi r3, r2, -22864
    li r4, 0x485
    addi r5, r2, -22856
    bl fn_80196E10
@L_801AEB34:
    mr r3, r28
    li r4, 0x0
    li r5, 0xc
    bl memset
    stw r28, 0x0(r26)
    lwz r0, 0x0(r27)
    addi r27, r27, 0x8
    lwz r3, 0x0(r26)
    stw r0, 0x4(r3)
    lwz r26, 0x0(r26)
@L_801AEB5C:
    lwz r0, 0x4(r27)
    cmplwi r0, 0x0
    bne @L_801AEB10
    lwz r0, 0x8(r1)
@L_801AEB6C:
    stw r0, 0xc(r31)
    li r3, -0x1
    li r0, 0x1
    stw r3, 0x10(r31)
    stb r0, 0x14(r31)
@L_801AEB80:
    lwz r0, 0x4(r31)
    rlwinm r0, r0, 0, 4, 0
    stw r0, 0x4(r31)
    b @L_801AEBC0
@L_801AEB90:
    lwz r3, 0x8(r30)
    lfs f0, 0x0(r3)
    stfs f0, 0x8(r31)
    lwz r3, 0x8(r30)
    lfs f0, 0x4(r3)
    stfs f0, 0xc(r31)
    b @L_801AEBC0
@L_801AEBAC:
    lis r4, -32729
    addi r3, r2, -22864
    addi r5, r4, 21128
    li r4, 0x37d
    bl fn_80196D78
@L_801AEBC0:
    mr r3, r31
    b @L_801AEBCC
@L_801AEBC8:
    li r3, 0x0
@L_801AEBCC:
    lmw r26, 0x18(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
