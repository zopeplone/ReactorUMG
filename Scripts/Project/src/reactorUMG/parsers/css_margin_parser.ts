import * as UE from "ue";
import { convertLengthUnitToSlateUnit } from "./css_length_parser";

/**
 * Expands padding values into umg padding values from css padding values
 * @param paddingValues 
 * @returns umg padding values
 */
export function expandPaddingValues(paddingValues: number[]): number[] {
    if (paddingValues.length === 2) {
        return [paddingValues[0], paddingValues[1], paddingValues[0], paddingValues[1]];
    } else if (paddingValues.length === 1) {
        return [paddingValues[0], paddingValues[0], paddingValues[0], paddingValues[0]];
    } else if (paddingValues.length === 3) {
        // padding: top right bottom
        return [paddingValues[0], paddingValues[1], paddingValues[2], paddingValues[1]];
    } else if (paddingValues.length === 4) {
        return paddingValues;
    } else if (paddingValues.length === 0) {
        return [0, 0, 0, 0];
    }

    return paddingValues;
}

export function convertToUEMargin(
    style: any,
    margin: string | number,
    top: string | number,
    right: string | number,
    bottom: string | number,
    left: string | number
): UE.Margin {

    const isProvided = (v: any) => v !== undefined && v !== null && !(typeof v === 'string' && v.trim() === '');

    if (!isProvided(margin) && !isProvided(top) &&
        !isProvided(right) && !isProvided(bottom) && !isProvided(left)) {
        return null;
    }

    const toSU = (v: any): number => {
        if (typeof v === 'number' || typeof v === 'string') {
            return convertLengthUnitToSlateUnit(v, style);
        }
        return 0;
    };

    // Normalize margin values
    let marginValues: number[] = [];
    if (typeof margin === 'number') {
        marginValues = [toSU(margin)];
    } else if (typeof margin === 'string') {
        const trimmed = margin.trim();
        if (trimmed.length > 0) {
            marginValues = trimmed.split(' ').map(v => toSU(v.trim()));
        }
    }

    let expandedMarginValues = expandPaddingValues(marginValues);

    if (isProvided(top)) {
        expandedMarginValues[0] = toSU(top);
    }
    if (isProvided(right)) {
        expandedMarginValues[1] = toSU(right);
    }
    if (isProvided(bottom)) {
        expandedMarginValues[2] = toSU(bottom);
    }
    if (isProvided(left)) {
        expandedMarginValues[3] = toSU(left);
    }

    // React Padding: top right bottom left
    // UMG Padding: Left, Top, Right, Bottom
    return new UE.Margin(
        expandedMarginValues[3],
        expandedMarginValues[0],
        expandedMarginValues[1],
        expandedMarginValues[2]
    );
}

export function convertPadding(style: any): UE.Margin {
    const padding = style?.padding;
    const paddingLeft = style?.paddingLeft;
    const paddingRight = style?.paddingRight;
    const paddingTop = style?.paddingTop;
    const paddingBottom = style?.paddingBottom;
    
    return convertToUEMargin(style, padding, paddingTop, paddingRight, paddingBottom, paddingLeft);
}

export function convertMargin(style: any): UE.Margin {
    const margin = style?.margin;
    const marginLeft = style?.marginLeft;
    const marginRight = style?.marginRight;
    const marginTop = style?.marginTop;
    const marginBottom = style?.marginBottom;

    return convertToUEMargin(style, margin, marginTop, marginRight, marginBottom, marginLeft);
}

export function convertGap(gap: any, style: any): UE.Vector2D {
    if (gap === undefined || gap === null || gap === '') {
        return new UE.Vector2D(0, 0);
    }

    const toSlate = (value: any): number => {
        if (value === undefined || value === null) {
            return 0;
        }
        return convertLengthUnitToSlateUnit(value, style);
    };

    let gapValues: number[] = [];

    if (Array.isArray(gap)) {
        gapValues = gap.map(toSlate);
    } else if (typeof gap === 'number') {
        gapValues = [toSlate(gap)];
    } else if (typeof gap === 'string') {
        const trimmed = gap.trim();
        if (trimmed.length > 0) {
            gapValues = trimmed.split(/\s+/).map(v => toSlate(v));
        }
    } else {
        // Unsupported type, treat as zero
        return new UE.Vector2D(0, 0);
    }

    if (gapValues.length === 2) {
        // gap: row column
        // innerSlotPadding: x(column) y(row)
        return new UE.Vector2D(gapValues[1], gapValues[0]);
    }

    const fallback = gapValues.length >= 1 ? gapValues[0] : 0;
    return new UE.Vector2D(fallback, fallback);
}
