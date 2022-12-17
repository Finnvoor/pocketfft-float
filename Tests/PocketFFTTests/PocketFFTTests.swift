//
//  PocketFFTTests.swift
//  
//
//  Created by Finn Voorhees on 16/12/2022.
//

import XCTest
@testable import PocketFFT

final class PocketFFTTests: XCTestCase {
    func err(_ a: [Float], _ b: [Float]) -> Float {
        var sum: Float = 0
        var errSum: Float = 0
        for (a, b) in zip(a, b) {
            errSum += (a - b) * (a - b)
            sum += b * b
        }
        return sqrt(errSum / sum)
    }
    
    func testReal() throws {
        var random = (0..<8192).map { _ in Float.random(in: 0...50_000)}
        var data = [Float](repeating: 0, count: 8192)
        for length in 1..<8192 {
            memcpy(&data, &random, length * MemoryLayout<Float>.size)
            let plan = make_rfft_plan(length)
            rfft_forward(plan, &data, 1)
            rfft_backward(plan, &data, 1 / Float(length))
            destroy_rfft_plan(plan)
            XCTAssertLessThan(
                err([Float](data[0..<length]), [Float](random[0..<length])),
                2e-6,
                "Problem at real length: \(length)"
            )
        }
    }
    
    func testComplex() throws {
        var random = (0..<8192 * 2).map { _ in Float.random(in: 0...50_000)}
        var data = [Float](repeating: 0, count: 8192 * 2)
        for length in 1..<8192 {
            memcpy(&data, &random, 2 * length * MemoryLayout<Float>.size)
            let plan = make_cfft_plan(length)
            cfft_forward(plan, &data, 1)
            cfft_backward(plan, &data, 1 / Float(length))
            destroy_cfft_plan(plan)
            XCTAssertLessThan(
                err([Float](data[0..<length * 2]), [Float](random[0..<length * 2])),
                2e-6,
                "Problem at real length: \(length)"
            )
        }
    }
}
